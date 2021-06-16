// ***************************************************************************
// **
// ** Motor controller for Dolphin Dynamic 2002 Pool Cleaner
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **
// **   15-Nov-2015
// **
// ***************************************************************************

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdint.h>

// Bit mask for robot direction control (MOTORDIR):
//   bit 3: motor 1 pos
//   bit 2: motor 1 neg
//   bit 1: motor 2 pos
//   bit 0: motor 2 neg
#define MOT_STOP  0b0000 // STOP    - both motors stopped.
#define MOT_FWD   0b1001 // FORWARD - both motors forward.
#define MOT_REV   0b0110 // REVERSE - both motors reverse.
#define MOT_LEFT  0b0101 // LEFT    - left motor forward, right motor reverse.
#define MOT_RIGHT 0b1010 // RIGHT   - left motor reverse, right motor forward.

typedef uint8_t MOTORDIR;

#define MOTOR_MAX_SPEED 1023

//void waitSeconds(uint8_t s);

void motorInit(void);
//void motorTick(void);
void motorSetDirection(MOTORDIR dir);
void motorSetDriveSpeed(uint16_t val);

//const char *motorGetStr(void);

// Put this in the SYSTICK block of the ISR.
// It handles timing of motor acceleration.
extern volatile uint16_t g_motorStepTime;
#define MOTOR_SYSTICK_ISR if(g_motorStepTime > 0) \
	g_motorStepTime--;

#endif
