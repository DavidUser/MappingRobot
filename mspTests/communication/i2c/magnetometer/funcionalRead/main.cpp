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

void configureTWI();
void beginTWI();
void endTWI();
void sendTWI(char);
void sendTWI(char[], unsigned int);
void readTWI(char[], unsigned int);
	 
main() {
	configureDCO();
	configureUART();
	configureTWI();


	char normalMeasurement8Averange75Hz[] = { 0x00, 0x78 };
	char gain[] = 				{ 0x01, 0xa0 };
	char operationMode[] = 			{ 0x02, 0x01 };

	beginTWI();
	sendTWI(normalMeasurement8Averange75Hz, 2);
	sendTWI(gain, 2);
	sendTWI(operationMode, 2);
	endTWI();
	__delay_cycles(6000);

	// Master Receiver

	while (true) {
		beginTWI();
		sendTWI(0x03);

		char received[6];
		readTWI(received, 6);
		//__delay_cycles(1000000/2);
		endTWI();

		short x, y, z;
		x = (static_cast<short>(received[0]) << 8) | received[1];
		z = (static_cast<short>(received[2]) << 8) | received[3];
		y = (static_cast<short>(received[4]) << 8) | received[5];
		print("( ");
		print(x);
		print(", ");
		print(y);
		print(", ");
		print(z);
		print(" )");
		print("\r\n");
	}

	// Ending I2C
	
	// if want stop transmission
	// XXX this line never will be executed in this context, because we have a infinit loop above
	UCB0CTL1 |= UCTXSTP; // setting UCTXSTP, STOP condition is generated

	// if want change slave address or transmitter/reciver mode
	// setting UCTXSTT communicate RESTART

	// if not-acknowledge was recived UCNACKIFG is set
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
		//beginTWI();



void configureTWI() {
	UCB0CTL1 = UCSWRST; // disable

	//configure i2 pins
	P1SEL |= BIT6 | BIT7;
	P1SEL2|= BIT6 | BIT7;


	UCB0CTL0 =  		// USCI_B0 control register
		UCMST		// set master mode	
		| UCMODE_3 	// I2C mode select
		| UCSYNC; 	// synchronous


	UCB0CTL1 |= 		// UCSI_B0 control register
		UCSSEL_2;	//select clock source, SMCLK

	UCB0I2CSA = 0x1e;	// write desired slave address to the UCBxI2CSA, magnetometer address
	
	// prescaler to generate 75 Hz (1MHz / 13333Hz)
	// UCB0BR0 = 0x15;
	// UCB0BR1 = 0x34;
	UCB0BR0 = 10;
	UCB0BR1 = 0;

	// select size of addres with UCSLA10, 7 bits default
	
	UCB0CTL1 &= ~UCSWRST; // enable
	print("[system] I2C configured\r\n");
}


void beginTWI() {
	UCB0CTL1 |=
		  UCTR		// setting UCTR for transmitter mode
		| UCTXSTT; 	// setting UCTXSTT to generate START condition
	__delay_cycles(6000);

	//XXX this is needed to a new transaction
	UCB0CTL1 &= ~UCSWRST; // enable
}

void endTWI() {
	UCB0CTL1 |= UCTXSTP; // send stop
	while (UCB0CTL1 & UCTXSTP); // wait STOP condition is ack

	//XXX this is needed to a new transaction
	UCB0CTL1 |= UCSWRST; // disable, stop i2c communication, high impendance SDA and SCL, clear i2c status register
}

void sendTWI(char byte) {
	sendTWI(&byte, 1);
}

void sendTWI(char byte[], unsigned int size) {
	for (typeof(size) i = 0; i < size; ++i) {
		while ( !(IFG2 & UCB0TXIFG) ); // wait UCBxTXIFG are set, when set the buffer are empty
		UCB0TXBUF = byte[i]; // load data to be trasmitted on UCBxTXBUF
	}
}

void readTWI(char byte[], unsigned int size) {
	if ( UCB0CTL1 & UCTR ) {
		UCB0CTL1 &= ~UCTR;	// setting UCTR for receiver mode
		UCB0CTL1 |= UCTXSTT; 	// setting UCTXSTT to generate START condition
		while (UCB0CTL1 & UCTXSTT);
	}

	for (typeof(size) i = 0; i < size; ++i) {
		while (!(IFG2 & UCB0RXIFG) );
		byte[i] = UCB0RXBUF;
	}

	UCB0CTL1 |= UCTXSTP; // send stop
	while (UCB0CTL1 & UCTXSTP); // wait STOP condition is ack
}


