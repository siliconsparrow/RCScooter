// ***************************************************************************
// **
// ** Motor controller for Dolphin Dynamic 2002 Pool Cleaner
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **
// **   15-Nov-2015
// **
// ***************************************************************************


// This module perform many functions to control the three motors:
//
//   1. Controls PWM1 to run the impeller motor at 18.6V, 1.8A.
//   2. Controls PWM2 to keep the directional motors supplied with 15.8V.
//   3. Monitors the current to all motors and shut down if there is an overload.
//   4. Uses the H-bridge to switch the direction of the directional motors.
//

#include "motor.h"
#include "pwm.h"
#include "board.h"
#include "systick.h"
#include <stdio.h>

//#include "serial.h"
//#include "systick.h"
/*
// Motor voltages.
#define SUPPLYVOLTAGE   29
#define IMPELLERVOLTAGE 18
#define MOTORVOLTAGE    15
#define VOLT_TO_PWM(v) (((int16_t)v*1023)/SUPPLYVOLTAGE)
*/

// Motor control rules:
// Only switch direction if motor is stopped.
// Motor speed must increase or decrease gradually - 2 seconds to go from full stop to full speed.
// Both drive motors share the same speed control.

// Acceleration timing.
//#define MOTOR_ACCEL_STEP_TICKS SYSTICK_MS_TO_TICKS(20)
//#define MOT_PWM_STEP 100

// Timing variable updated in the systick ISR.
//volatile uint16_t g_motorStepTime = 0;

// Motor state.
//uint16_t m_drivePWM;           // Current PWM setting of the drive motors.
//uint16_t m_drivePWMtarget;     // Desired PWM setting of the drive motors.
//MOTORDIR m_driveDirection;     // Current drive direction.
//MOTORDIR m_driveTarget;        // Desired drive direction.


// ***************************************************************************
// ** Private functions.
// **

// Switches the transistors on the H-bridge to
// set the directions of the two drive motors.
void motorSwitchDirection(MOTORDIR dir)
{
	// Break before make.
	MOT1_CTL1 = 0;
	//MOT1_CTL2 = 0;
	MOT2_CTL1 = 0;
	//MOT2_CTL2 = 0;

	// Set up the new direction.
	MOT1_CTL1 = 0 != (dir & 0x08);
	MOT1_CTL2 = 0 != (dir & 0x04);
	//MOT2_CTL1 = 0 != (dir & 0x02);
	//MOT2_CTL2 = 0 != (dir & 0x01);
}


// ***************************************************************************
// ** Public functions.
// **

// Set up hardware and PWMs to control all the motors.
void motorInit(void)
{
	// Set up PWMs for motor speed control.
	pwmInit();

	// Set up data directions for H-Bridge.
	MOT1_CTL1_DDR = OUTPUT;
	MOT1_CTL2_DDR = OUTPUT;
	MOT2_CTL1_DDR = OUTPUT;
	MOT2_CTL2_DDR = OUTPUT;

	// Set all motors to STOP.
  	MOT1_CTL1 = 0;
	MOT1_CTL2 = 0;
	MOT2_CTL1 = 0;
	MOT2_CTL2 = 0;

	// Set all motors to STOP.
	//m_drivePWM = m_drivePWMtarget = 0;
	//motorSwitchDirection(m_driveTarget = m_driveDirection = MOT_STOP);
}

/*
// Called frequently from the main program loop, this handles the acceleration
// of the motors.
void motorTick(void)
{
	if(g_motorStepTime == 0) // Is it time to perform another motor update?
	{
		g_motorStepTime = MOTOR_ACCEL_STEP_TICKS;

		// Are the drive motors going in the right direction?
		if(m_driveTarget != m_driveDirection)
		{
			// Can't change direction unless speed is zero.
			if(m_drivePWM == 0)
			{
				// Drive motors are stopped. It's ok to change direction now.
				m_driveDirection = m_driveTarget;
				motorSwitchDirection(m_driveTarget);
			}
			else
			{
				// Drive speed is non-zero. Slow it down.
				m_drivePWM -= MOT_PWM_STEP;
				pwm2SetDuty(m_drivePWM);
			}
		}
		else
		{
			// Drive motors are the right direction but are they too slow?
			if(m_drivePWM < m_drivePWMtarget)
			{
				// Speed up drive motors.
				m_drivePWM += MOT_PWM_STEP;
				pwm2SetDuty(m_drivePWM);
			}
		}
	}
}
*/

// Set the steering direction. The interrupt handler and motorTick() will handle the
// actual motor control.
void motorSetDirection(MOTORDIR dir)
{
    motorSwitchDirection(dir);
	//m_driveTarget = dir;
}

// Set the PWM duty cycle of the drive motors.
void motorSetDriveSpeed(uint16_t val)
{
    pwm2SetDuty(val);
	//m_drivePWMtarget = val;
}

/*
// Return status of all motors as a human-readable string.
const char *motorGetStr(void)
{
	static char result[40];

	const char *dir1;
	const char *dir2;

	if(MOT1_CTL1 == MOT1_CTL2)
		dir1 = "---";
	else if(MOT1_CTL1)
		dir1 = "FWD";
	else
		dir1 = "REV";

	if(MOT2_CTL1 == MOT2_CTL2)
		dir2 = "---";
	else if(MOT2_CTL1)
		dir2 = "FWD";
	else
		dir2 = "REV";

	sprintf(result, "%4d %s %s", (int)m_drivePWM, dir1, dir2);
	return result;
}
*/
