#include "mbed.h"

DigitalIn pirSensor(P0_0, PullUp);
DigitalOut led(P0_6);

int main() {
	wait(2);

    while (1) {
    	if (!pirSensor) {
    		led = 1;
    		wait(2);
    	} else {
    		led = 0;
    	}
    }

    return 0;
}
