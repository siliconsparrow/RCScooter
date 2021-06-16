// ***************************************************************************
// **
// **  Useful timer functions for PIC18.
// **
// **  by Adam Pierce <adam@siliconsparrow.com>
// **  created 13-Mar-2016
// **
// ***************************************************************************

#include "board.h"
#include "systick.h"

volatile uint32_t g_systick = 0;

// Set up Timer 2 with interrupt.
// the systick global variable is incremented every interrupt.
void systickInit(void)
{
	// Set timer to interrupt at (Fosc/4/16/250) = 250Hz for Fosc=4MHz.
	T2CON = 0b00000011; // Prescale=16.
	PR2   = 250;

	// Enable timer interrupt.
	PIE1bits.TMR2IE = 1;

	// Start the timer.
	T2CONbits.TMR2ON = 1;
}

uint32_t systickGet(void)
{
	uint32_t result;

	INTERRUPTS_DISABLE;
	result = g_systick;
	INTERRUPTS_ENABLE;
	return result;
}

// Block for the given number of seconds.
void systickWaitSeconds(uint8_t s)
{
    systickWaitTicks((uint32_t)s * SYSTICK_FREQ);
}

void systickWaitTicks(uint16_t ticks)
{
	uint32_t tStart;
	uint32_t tNow;
    
    tStart = systickGet();
	while(1)
	{
		tNow = systickGet();
		tNow -= tStart;
		if(tNow >= ticks)
			break;
	}
}