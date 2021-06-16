// *************************************************************************
// **
// ** Serial communication functions for PIC18.
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   created 11-Jul-2012
// **
// *************************************************************************

#include "board.h"
#include "serial.h"

volatile uint8_t rxbuf[RXBUFSIZE];
volatile uint8_t txbuf[TXBUFSIZE];
volatile uint8_t txbufstart;
volatile uint8_t txbufend;
volatile uint8_t rxbufstart;
volatile uint8_t rxbufend;

// Set up serial port with the given baud rate,n,8,1
void serialInit(unsigned brg)
{
	// Disable the serial port while we're configuring it.
	RCSTAbits.SPEN = 0;

	// Initialise variables
	txbufstart = txbufend = 0;
	rxbufstart = rxbufend = 0;

	// Set data directions
	TRISCbits.RC7  = 1;
	TRISCbits.RC6  = 1;

	// Set baud rate
	SPBRGH  = brg >> 8;
	SPBRG   = brg & 0xFF;

	// Set UART options (8 data bits, asynchronous, non-inverted, receive and transmit)
	TXSTA   = 0b00100100;
	RCSTA   = 0b00010000;
	BAUDCON = 0b00001010;

	// Enable the serial receive interrupt.
	PIE1bits.RCIE = 1;

	RCSTAbits.SPEN = 1;
}

// Change the baud rate.
void serialSetBaud(unsigned brg)
{
	SPBRGH = brg >> 8;
	SPBRG  = brg & 0xFF;
}

// Puts a byte in the transmit queue. If the queue is full, it will
// block until there is room.
void serialSend(uint8_t b)
{
	uint8_t x;

	// Block until there is free space in the queue.
	while(txbufstart == (x = (txbufend + 1) & TXBUFMASK))
		;

	txbuf[txbufend] = b;
	txbufend = x;

	// Hand transmission over to the ISR.
	PIE1bits.TXIE = 1;
}

// Get a byte from the receive queue. If the queue is empty,
// it will block until a byte arrives.
uint8_t serialReceive(void)
{
	uint8_t b;

	while(rxbufstart == rxbufend)
		;

	b = rxbuf[rxbufstart];
	rxbufstart = (rxbufstart + 1) & RXBUFMASK;

	return b;
}

// Blocks until transmit buffer is empty. Also clears the receive buffer.
void serialFlush(void)
{
	// Clear receive buffer
	rxbufstart = rxbufend = 0;

	// Wait for software queue to empty.
	while(txbufstart != txbufend)
		;

	// Wait for hardware shift register to empty.
	while(TXSTAbits.TRMT == 0)
		;
}

// Send data bytes.
void serialSendData(const uint8_t *data, int size)
{
	int i;

	for(i = 0; i < size; i++)
		serialSend(data[i]);
}

// Send a string from a RAM pointer.
void serialSendString(const char *str)
{
	while(*str)
		serialSend((uint8_t)*str++);
}
