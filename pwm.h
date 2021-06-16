// ***************************************************************************
// **
// ** PWM driver for PIC18F2420
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **
// **   15-Nov-2015
// **
// ***************************************************************************

#ifndef _PWM_H_
#define _PWM_H_

void pwmInit(void);

#define PWM_MAX 1023

#define pwm1SetDuty(d) { CCP1CON=(CCP1CON&0xCF)|((d&3)<<4); CCPR1L=(d)>>2; }
#define pwm2SetDuty(d) { CCP2CON=(CCP2CON&0xCF)|((d&3)<<4); CCPR2L=(d)>>2; }

#endif
