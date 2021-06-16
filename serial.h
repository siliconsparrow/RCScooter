// *************************************************************************
// **
// ** Serial communication functions for PIC18.
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   created 11-Jul-2012
// **
// *************************************************************************

#ifndef _UART_H_
#define _UART_H_

#include "board.h"
#include <stdint.h>

// Baud rate formula: FOSC/[4 (n + 1)] when BRGH=1 and BRG16=1
#define BRGVAL(b) ((((CPU_FOSC)/4)/(b))-1)
#define BAUD300    BRGVAL(300)
#define BAUD1200   BRGVAL(1200)
#define BAUD4800   BRGVAL(4800)
#define BAUD9600   BRGVAL(9600)
#define BAUD19200  BRGVAL(19200)
#define BAUD38400  BRGVAL(38400)
#define BAUD57600  BRGVAL(57600)
#define BAUD115200 BRGVAL(115200)

// Macros to temporarily disable the UART to save power.
#define SERIAL_SUSPEND { PIE1bits.RCIE = 0; RCSTAbits.SPEN = 0; }
#define SERIAL_RESUME  { RCSTAbits.SPEN = 1; PIE1bits.RCIE = 1; }

void    serialInit(unsigned brg);
void    serialSetBaud(unsigned brg);
void    serialSend(uint8_t b);
uint8_t serialReceive(void);
void    serialFlush(void);
void    serialSendData(const uint8_t *data, int size);
void    serialSendString(const char *str);

// Simple check to see if there are received bytes in the buffer.
#define serialHasRxBytes() (rxbufend != rxbufstart)

// Calculate the number of bytes in the receive buffer.
#define serialBytesAvailable() (((rxbufend + RXBUFSIZE) - rxbufstart) & RXBUFMASK)

// RXBUFSIZE and TXBUFSIZE must be a power of 2.

#define RXBUFSIZE 32
#define TXBUFSIZE 32

#define RXBUFMASK (RXBUFSIZE - 1)
#define TXBUFMASK (TXBUFSIZE - 1)

extern volatile uint8_t rxbuf[];
extern volatile uint8_t txbuf[];
extern volatile uint8_t txbufstart;
extern volatile uint8_t txbufend;
extern volatile uint8_t rxbufstart;
extern volatile uint8_t rxbufend;

// Set the bit to wake on serial receive.
// This must be done every time you go into SLEEP mode.
#define SERIAL_WAKE_ON_RECV BAUDCONbits.WUE = 1

// Serial interrupt handler
#define SERIAL_ISR { \
	if(PIR1bits.RCIF) \
	{ \
		if(RCSTAbits.OERR) { \
			RCSTAbits.CREN = 0; \
			RCSTAbits.CREN = 1; \
		} \
		rxbuf[rxbufend] = RCREG; \
		x = (rxbufend + 1) & RXBUFMASK; \
		if(x != rxbufstart) \
			rxbufend = x; \
	} \
	if(PIR1bits.TXIF) \
	{ \
		if(txbufstart != txbufend) \
		{ \
			TXREG = txbuf[txbufstart]; \
			txbufstart = (txbufstart + 1) & TXBUFMASK; \
		} \
		else if(TXSTAbits.TRMT == 1) \
		{ \
			PIE1bits.TXIE = 0; \
			RCSTAbits.CREN = 1; \
		} \
	} \
}

#endif
