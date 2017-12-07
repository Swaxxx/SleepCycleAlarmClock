#include "mbed.h"

Serial usbConnection(USBTX, USBRX);

int main() {
	int i = 0;

	usbConnection.baud(9600);

    while (1) {
		i++;
		usbConnection.printf("[%d] Connection establised\r\n", i);
		wait(1);
    }

    return 0;
}
