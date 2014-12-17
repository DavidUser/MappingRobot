/* 
 * Print all printable characters on usb channel using UART mode of USCI module
 * the print is based in busy wait to transfer characters
 */

#include "msp/msp430g2553.h"

void configureInterface() {
	P1DIR |= BIT0;
	P1DIR &= ~BIT4;
	P1REN &= ~BIT4;
	P2DIR &= ~(BIT0 | BIT1);
	P2REN &= ~(BIT0 | BIT1);
	P1OUT |= BIT0; // busy report
}

int receiveCommand() {
	P1OUT &= ~BIT0; // clear busy report
	while (!(P1IN & BIT4)); // wait receive command report
	int command = (P2IN & (BIT0 | BIT1));
	P1OUT |= BIT0; // busy report
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
