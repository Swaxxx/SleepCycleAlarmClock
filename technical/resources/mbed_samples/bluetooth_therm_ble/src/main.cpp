#include "mbed.h"
#include "ble/BLE.h"
#include "ble/services/HealthThermometerService.h"

DigitalOut led1(LED1);

static HealthThermometerService *thermometerServicePtr;

static const char     DEVICE_NAME[]        = "Therm";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEALTH_THERMOMETER_SERVICE};
static volatile bool  triggerSensorPolling = false;
static float          currentTemperature   = 39.6;

/* Restart Advertising on disconnection*/
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params) {
    BLE::Instance().gap().startAdvertising();
}

void periodicCallback(void) {
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */

    /* Note that the periodicCallback() executes in interrupt context, so it is safer to do
     * heavy-weight sensor polling from the main thread. */
    triggerSensorPolling = true;
}

/**
 * This function is called when the ble initialization process has failed
 */
void onBleInitError(BLE &ble, ble_error_t error) {
    /* Avoid compiler warnings */
    (void) ble;
    (void) error;
    /* Initialization error handling should go here */
}

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params) {
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup primary service. */
    thermometerServicePtr = new HealthThermometerService(ble, currentTemperature, HealthThermometerService::LOCATION_EAR);

    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::THERMOMETER_EAR);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    ble.gap().startAdvertising();    
}

int main(void) {
    led1 = 1;
    Ticker ticker;
    ticker.attach(periodicCallback, 1);

    BLE &ble = BLE::Instance();
    ble.init(bleInitComplete);
    
    /* SpinWait for initialization to complete. This is necessary because the
     * BLE object is used in the main loop below. */
    while (ble.hasInitialized()  == false) { /* spin loop */ }

    while (true) {
        if (triggerSensorPolling && ble.gap().getState().connected) {
            triggerSensorPolling = false;

            /* In our case, we simply update the dummy temperature measurement. */
            currentTemperature += 0.1;
            thermometerServicePtr->updateTemperature(currentTemperature);
        } else {
            ble.waitForEvent();
        }
    }
}
