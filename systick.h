// ***************************************************************************
// **
// **  Useful timer functions for PIC18.
// **
// **  by Adam Pierce <adam@siliconsparrow.com>
// **  created 13-Mar-2016
// **
// ***************************************************************************

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "board.h"
#include <stdint.h>

extern volatile uint32_t g_systick;

void     systickInit(void);
uint32_t systickGet(void);
void     systickWaitSeconds(uint8_t s);
void     systickWaitTicks(uint16_t ticks);
        
#define SYSTICK_MS_TO_TICKS(ms)   ((((uint32_t)ms)*SYSTICK_FREQ)/1000)
#define SYSTICK_SEC_TO_TICKS(sec) (((uint32_t)sec)*SYSTICK_FREQ)

#define SYSTICK_FREQ ((((uint32_t)CPU_FOSC/4)/16)/256)

// Handle the timer2 interrupt.
#define SYSTICK_ISR_START if(PIR1bits.TMR2IF) { \
	PIR1bits.TMR2IF = 0; \
	g_systick++;

#define SYSTICK_ISR_END }

#endif
