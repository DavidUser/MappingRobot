/* 
 * Print all printable characters on usb channel using UART mode of USCI module
 * the print is based in busy wait to transfer characters
 */

#include "msp/msp430g2553.h"

#define INTERFACE_PORT_OUT 	P1OUT
#define INTERFACE_PORT_IN 	P1IN
#define INTERFACE_PORT_DIR 	P1DIR
#define CMD0	BIT0
#define CMD1	BIT1
#define TX	BIT2
#define RX	BIT3

void configureInterface() {
	INTERFACE_PORT_DIR &= ~(CMD0 | CMD1 | RX);
	INTERFACE_PORT_DIR |= TX; // busy report
}

int receiveCommand() {
	INTERFACE_PORT_OUT &= ~TX; // clear busy report
	while (!(INTERFACE_PORT_IN & RX)); // wait receive command report
	int command = INTERFACE_PORT_IN & (CMD0 | CMD1);
	INTERFACE_PORT_OUT |= TX; // busy report
	return command;
}

main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT

	// Set DCO to 1MHz
	BCSCTL1 = CALBC1_1MHZ; 
	DCOCTL = CALDCO_1MHZ; 

	configureInterface();
	P1DIR |= BIT6; // led 2
	P1OUT &= ~BIT6;
	P1REN &= ~BIT6;

	while (true) {
		int command = receiveCommand();

		do {
			P1OUT ^= BIT6;
			__delay_cycles(500000);
			P1OUT ^= BIT6;
			__delay_cycles(500000);
		} while (command--);
	}
}
