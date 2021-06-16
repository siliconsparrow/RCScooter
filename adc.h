// *************************************************************************
// **
// ** Analogue to Digital functions for the PIC18F24K20
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   for Levaux Pty. Ltd.
// **   created 13-Feb-2014
// **
// *************************************************************************

#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

#define ADC_MAX 1023 // Value when ADC is at maximum.

void     adcInit(void);
void     adcDisable(void);
uint16_t adcRead(uint8_t channel);

// Start an ADC conversion. Interrupt will be raised when done.
//#define ADCREAD(chan) { \
//	ADCON0 = 1 | ((chan) << 2); \
//	ADCON0bits.GO = 1; \
//}

#endif
