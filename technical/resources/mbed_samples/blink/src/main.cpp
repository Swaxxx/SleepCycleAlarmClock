#include "mbed.h"

DigitalOut led(LED1);

int main() {
    while (1) {
		led = !led;
		wait(1);
    }

    return 0;
}

