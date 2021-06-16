// ***************************************************************************
// **
// ** PWM driver for PIC18F2420
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **
// **   15-Nov-2015
// **
// ***************************************************************************

#include "pwm.h"
#include "board.h"

// Init both PWM peripherals on the PIC18F2420.
// You must have called systickInit() previously to set up the timing.
void pwmInit(void)
{
	// Set port pins to output
	PWM1_OUT_DDR = OUTPUT;
	PWM2_OUT_DDR = OUTPUT;

	// Set CCP1 to PWM mode.
	CCPR1H = 0;           // Reset PWM duty cycle to zero.
	CCPR1L = 0;
	CCP1CON = 0b00001100; // Select single-output PWM mode.
	
	// Set CCP2 to PWM mode.
	CCPR2H = 0;           // Reset PWM duty cycle to zero.
	CCPR2L = 0;
	CCP2CON = 0b00001100; // Select single-output PWM mode.

	// Set to 0% duty cycle.
	pwm1SetDuty(0);
	pwm2SetDuty(0);
}
