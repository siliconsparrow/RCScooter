// ***************************************************************************
// **
// ** RCScooter
// ** Interfaces a radio control receiver to an H-Bridge motor controller
// **
// **   for the PIC182420
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   7-Jun-2021
// **
// ***************************************************************************

#include "board.h"
#include "LED.h"
#include "systick.h"
#include "serial.h"
#include "pwm.h"
#include "motor.h"
#include "adc.h"
#include "pwmSense.h"
#include <stdio.h>

// What this is supposed to do
    // Detect incoming PWM pulses across 3 channels using timer compare.
    // Timer compare interrupt sets analogue levels.
    // Channel 1 will output an analogue levels via PWM (an RC circuit will smooth it out)
    // Channel 2 will control the H-Bridge speed and direction
    // Channel 3 is reserved for future use

// Define this to print debug info to the UART at 9600bps.
//#define UART_OUTPUT

#define RC_MIN            60 // Remote control channel typical minimum value.
#define RC_MAX           185 // Remote control channel typical maximum value.
#define RC_CENTRE        130 // Initial value - represents centre of control's travel.
#define RC_CHAN_STEERING   0
#define RC_CHAN_THROTTLE   1
#define RC_CHAN_SPARE      2

#define STEER_DEADBAND_LOW  110
#define STEER_DEADBAND_HIGH 130

#define STEER_CURRENT_MAX_MA 2000 // Maximum milliamps for steering actuator

//#define CURRENT_SENSE_MAX ((uint16_t)(((uint32_t)STEER_CURRENT_MAX_MA*ADC_MAX)/5000))

// Despite my calculations, stall current seems to be about 300 on the ADC
#define CURRENT_SENSE_MAX 300

void main(void)
{
    uint16_t steerCurrent;
    uint32_t throttle;
    uint8_t  steer;
    char     buf[64];
        
	boardInit();          // Set up the microprocessor.
    systickInit();        // Set up timer.
    adcInit();
    ledInit();            // Set up flashy light.
    pwmInit();            // Set up PWM for traction motor control.
    pwmSenseInit();       // Set up PWM receiver for the R/C input.
    motorInit();          // Set up H-bridge motor controller

#ifdef UART_OUTPUT
	serialInit(BAUD9600); // Set up serial port.
#endif

    // Enable watchdog timer (timeout is set in the config flags in board.c)
    WDTCONbits.SWDTEN = 1;
    
	// Interrupts are used for the serial port, system timer and to
	// detect over-current on the motors.
	INTERRUPTS_ENABLE;

    serialSendString("Remote Control Scooter\r\n");
    
    // Set the speed of the steering actuator.
    motorSetDriveSpeed(MOTOR_MAX_SPEED);
    
    // Main loop
    while(1) {
        ClrWdt();
        
        // Handles acceleration and direction of steering motor.
        //motorTick();

#ifdef UART_OUTPUT
        // Print status to the UART        
        for(uint8_t i = 0; i < PWM_SENSE_CHANNELS; i++) {
            sprintf(buf, " CH%d=%d", i, pwmSenseGet(i));
            serialSendString(buf);
        }
        serialSendString("  ");
        //serialSendString(motorGetStr());
        sprintf(buf, " I=%d", steerCurrent);
        serialSendString(buf);
        serialSendString("\r\n");
#endif
                
        // Remote control channel 0 is the steering.
        steer = pwmSenseGet(RC_CHAN_STEERING);
        if(steer >= RC_MIN) {
            if(steer < STEER_DEADBAND_LOW) {
                motorSetDirection(MOT_REV);
            } else if(steer > STEER_DEADBAND_HIGH) {
                motorSetDirection(MOT_FWD);
            } else {
                motorSetDirection(MOT_STOP);
            }
        }
        
        // Stop motor if over-current
        steerCurrent = adcRead(ADC_CHAN_STEER);
        if(steerCurrent >= CURRENT_SENSE_MAX) {
            motorSetDirection(MOT_STOP);
        }
        
        // Remote control channel 1 is the throttle.
        throttle = pwmSenseGet(RC_CHAN_THROTTLE);
        if(throttle == 0) {
            throttle = RC_CENTRE;
        }
        
        if(throttle >= RC_MIN) {
            throttle -= RC_MIN;
            throttle *= PWM_MAX;
            throttle /= (RC_MAX - RC_MIN);
            if(throttle > PWM_MAX) {
                throttle = PWM_MAX;
            }
            pwm1SetDuty(throttle);
        }
    
        // Remote control channel 2 is a single button. No idea what to do
        // with it yet.
        if(pwmSenseGet(RC_CHAN_SPARE) > 100) {
            ledOn();
        } else {
            ledOff();
        }
    }
}


#ifdef OLD
#include "motor.h" 
#include "systick.h"

// Preset motor speeds.
#define MAIN_MOTOR_SPEED  600
#define DRIVE_MOTOR_SPEED 300

// Define this symbol to use serial control.
//#define MANUAL_CONTROL


// Do nothing for the given number of seconds. Makes sure that watchdog and
// background tasks are properly served while waiting.
void waitSeconds(int secs)
{
	uint32_t tStart;
	uint32_t tNow;
	uint32_t tTicks;
	
	tStart = systickGet();
	tTicks = SYSTICK_SEC_TO_TICKS(secs);
	
	while(1)
	{
		// Time up?
		tNow = systickGet();
		tNow -= tStart;
		if(tNow >= tTicks)
			return;

		// Do housekeeping stuff while we're waiting.
		ClrWdt();
		motorTick();
	}		
}

void main(void)
{
	int i,j;
	BYTE b;
	uint32_t lastPrint = 0;

	// Set up the microprocessor.
	boardInit();

	// Set up timer 2 to approximately 400Hz.
	systickInit();

	// Set up for motor control.
	motorInit();

	motorSetDriveSpeed(DRIVE_MOTOR_SPEED);
	motorSetImpellerSpeed(0);

	// Set up flashy light.
	LED0_DDR = OUTPUT;

	// Set up serial port.
	serialInit(BAUD9600);

	// Interrupts are used for the serial port, system timer and to
	// detect over-current on the motors.
	INTERRUPTS_ENABLE;

	serialSendString_pgm((PGMSTR)"Motor drive v1.1\r\n");

	LED0 = 1;

	// Main program loop.
	while(1)
	{
	#ifdef MANUAL_CONTROL
		ClrWdt();
		motorTick();

		if(serialBytesAvailable())
		{
			b = serialReceive();

			switch(b)
			{
			case ' ':
				motorSetDirection(MOT_STOP);
				motorSetImpellerSpeed(0);
				break;

			case 'f': motorSetDirection(MOT_FWD);   break;
			case 'b': motorSetDirection(MOT_REV);   break;
			case 'l': motorSetDirection(MOT_LEFT);  break;
			case 'r': motorSetDirection(MOT_RIGHT); break;

			default:
				if(b >= '0' && b <= '9')
					motorSetImpellerSpeed(70*((uint16_t)b-'0'));
			}
		}				

		if((systickGet() - lastPrint) >= (SYSTICK_FREQ/8))
		{
			lastPrint = systickGet();

			LED0 = ~LED0;

			serialSendString(motorGetStr());
			serialSendString_pgm((PGMSTR)"\r\n");
		}

	#else

		// Automatic mode sequence.
		for(i = 0; i < 4; i++)
		{
			LED0 = 1;
			motorSetImpellerSpeed(MAIN_MOTOR_SPEED);
			for(j = 0; j < 6; j++)
			{
				motorSetDirection(MOT_FWD);
				waitSeconds(60);
				motorSetDirection(MOT_REV);
				waitSeconds(60);
			}
			LED0 = 0;
			motorSetImpellerSpeed(0);
			motorSetDirection(MOT_LEFT);
			waitSeconds(5);
		}

		for(i = 0; i < 4; i++)
		{
			LED0 = 1;
			motorSetImpellerSpeed(MAIN_MOTOR_SPEED);
			for(j = 0; j < 6; j++)
			{
				motorSetDirection(MOT_FWD);
				waitSeconds(60);
				motorSetDirection(MOT_REV);
				waitSeconds(60);
			}
			LED0 = 0;
			motorSetImpellerSpeed(0);
			motorSetDirection(MOT_RIGHT);
			waitSeconds(5);
		}
	#endif
	}
}

#endif // OLD
