#include "mbed.h"

DigitalIn pirSensor(P0_0, PullUp);
DigitalOut led(P0_6);
Serial usbConnection(USBTX, USBRX);

int main() {
	int i = 0;

	usbConnection.baud(9600);

	wait(2);

    while (1) {
    	if (!pirSensor) {
    		led = 1;
    		i++;
			usbConnection.printf("[%d] Blink\r\n", i);
    		wait(2);
    	} else {
    		led = 0;
    	}
    }

    return 0;
}
