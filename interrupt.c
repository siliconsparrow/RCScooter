// ***************************************************************************
// **
// ** Interrupt Handler for the Pool Cleaner Controller
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **
// **   26-Mar-2015
// **
// ***************************************************************************

#include "systick.h"
#include "serial.h"
#include "pwmSense.h"
#include "motor.h"

//volatile uint16_t countdown;

void __interrupt() isr(void)
{
	uint8_t x;

	// Use the ADC to measure current from each of the three motors.
	// Check the current does not exceed the maximum.

	// Do I want to monitor the average voltage and adjust PWM?

    // Handle PWM sense.
    ISR_PWMSENSE;
   
	// System tick used for timing.
	SYSTICK_ISR_START;

	//MOTOR_SYSTICK_ISR;

	SYSTICK_ISR_END;

	// Handle serial I/O.
	SERIAL_ISR;
}
