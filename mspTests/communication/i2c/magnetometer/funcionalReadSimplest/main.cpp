/* 
 * Print all printable characters on usb channel using UART mode of USCI module
 * the print is based in busy wait to transfer characters
 */

#include "msp/msp430g2553.h"

void configureDCO();
  
void configureUART();
void putc(char);
void print (const char[]);
void print (short);
void printStatus();
	 
main() {
	configureDCO();
	configureUART();


	UCB0CTL1 = UCSWRST;  //Reset USCI

	UCB0CTL0 =  		// USCI_B0 control register
		UCMST		// set master mode	
		| UCMODE_3 	// I2C mode select
		| UCSYNC; 	// synchronous

	UCB0CTL1 = 		// UCSI_B0 control register
		UCSSEL_2	//select clock source, SMCLK
		| UCSWRST;

	UCB0I2CSA = 0x1e;	// write desired slave address to the UCBxI2CSA, magnetometer address
	
	// prescaler to generate 75 Hz (1MHz of SMCLK / 13981 of prescaler)
	UCB0BR0 = 0x9D;
	UCB0BR1 = 0x36;

	//configure ports
	P1SEL |= BIT6 | BIT7;
	P1SEL2|= BIT6 | BIT7;

	UCB0CTL1 &= ~UCSWRST; // Initializate USCI

	print("[system] I2C configured\r\n");

	// send start to transmit
	UCB0CTL1 |=
		  UCTR		// setting UCTR for transmitter mode
		| UCTXSTT; 	// setting UCTXSTT to generate START condition

	// point to register A ( address 0x00 )
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x00; // load data to be trasmitted on UCBxTXBUF

	// write 0x78 in register A, mean of eight measurements, 75 Hz sample rate
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x78; // load data to be trasmitted on UCBxTXBUF

	// point to register B ( address 0x01 )
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x01; // load data to be trasmitted on UCBxTXBUF
	
	// write 0xa0 in register B, gain five
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0xa0; // load data to be trasmitted on UCBxTXBUF

	// point to mode register ( address 0x02 )
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x02; // load data to be trasmitted on UCBxTXBUF

	// write 0x00 in mode register, continuous operation
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x00; // load data to be trasmitted on UCBxTXBUF
	
	// point to first data output, X MSB register
	while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
	UCB0TXBUF = 0x03; // load data to be trasmitted on UCBxTXBUF

	// send stop
	UCB0CTL1 |= UCTXSTP; // send stop
	while (UCB0CTL1 & UCTXSTP); // wait STOP condition is ack

	__delay_cycles(6000); // wait 6ms, until change the settings of magnetometer

	// send start to receive
	UCB0CTL1 &= ~UCTR;	// setting UCTR for receiver mode
	UCB0CTL1 |= UCTXSTT; 	// setting UCTXSTT to generate START condition
	while (UCB0CTL1 & UCTXSTT);

	// infinit loop, show continually the measuread values
	while (true) {
		// allocate space to the measured values, (x,y,z) coordinates, high byte (h prefix), low byte (l prefix)
		char hx, lx, hz, lz, hy, ly;

		// receive data output X MSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		hx = UCB0RXBUF; // load data from buffer to local memory

		// receive data output X LSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		lx = UCB0RXBUF; // load data from buffer to local memory

		// receive data output Z MSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		hz = UCB0RXBUF; // load data from buffer to local memory

		// receive data output Z LSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		lz = UCB0RXBUF; // load data from buffer to local memory

		// receive data output Y MSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		hy = UCB0RXBUF; // load data from buffer to local memory

		// receive data output Y LSB register
		while (!(IFG2 & UCB0RXIFG) ); // wait UCBxRXIFG are set, when set read buffer are full
		ly = UCB0RXBUF; // load data from buffer to local memory

		// compose (x, y, z) values
		short x, y, z;
		x = (static_cast<short>(hx) << 8) | lx; // compose x MSB and LSB
		z = (static_cast<short>(hz) << 8) | lz; // compose z MSB and LSB
		y = (static_cast<short>(hy) << 8) | ly; // compose y MSB and LSB

		//print (x, y, z) measured values
		print("( ");
		print(x);
		print(", ");
		print(y);
		print(", ");
		print(z);
		print(" )");
		print("\r\n");
	}
}


void configureDCO() {
	WDTCTL = WDTPW + WDTHOLD; // Stop WDT

	// Set DCO to 1MHz
	BCSCTL1 = CALBC1_1MHZ; 
	DCOCTL = CALDCO_1MHZ; 

	__delay_cycles(1000000);
}

void configureUART() {
	UCA0CTL1 |= UCSWRST; // put USCI in reset mode

	// P1.1 = RXD, P1.2=TXD
	P1SEL  |= BIT1 | BIT2;
	P1SEL2 |= BIT1 | BIT2;

	UCA0CTL1 |= UCSSEL_3; // Use SMCLK

	// Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13) 1MHz / 104
	UCA0BR0 = 104;
	UCA0BR1 = 0;

	UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1, correction for the fractional part ( the rest, 9600 - 1MHz / 104)
	UCA0CTL1 &= ~UCSWRST; // put USCI in operad mode (removing reset mode)

	/*End configuration hardware UART */
	print("[system] uart configured\r\n");
};
	

void putc(char ch) {
	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = ch;
}

void print (const char msg[]) {
	for (int i = 0; msg[i] != '\0'; ++i)
		putc(msg[i]);	
}

void print (short value) {
	if (value == 0) { putc('0'); return; }
	if (value < 0) {
		putc('-');
		value *= -1;
	}
	unsigned int size;
	char buffer[20];
	for (size = 0; value; ++size, value /= 10)
		buffer[size] = value % 10;
	while (size)
		putc(buffer[--size] + '0');
}

void printStatus() {
	print("Status:\n\r");
	#define __PRINT_STATUS(X, MSG) if (UCB0STAT & X) print(#MSG "! ");
	__PRINT_STATUS(UCNACKIFG, nack);
	__PRINT_STATUS(UCALIFG, arbitration lost);
	__PRINT_STATUS(UCGC, receive general call);
	__PRINT_STATUS(UCBBUSY, bus busy);
	__PRINT_STATUS(UCSCLLOW, SCL low);
	__PRINT_STATUS(UCSTPIFG, stop received);
	__PRINT_STATUS(UCSTTIFG, start received);
	print("done\n\r");
}
