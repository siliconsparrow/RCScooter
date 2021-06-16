/* 
 * File:   pwmSense.h
 * Author: adam
 *
 * Created on 9 June 2021, 9:05 PM
 */

#ifndef PWMSENSE_H
#define	PWMSENSE_H

#include <stdint.h>

#define PWM_SENSE_CHANNELS 3

extern volatile uint8_t pwmSenseValue[];

void     pwmSenseInit(void);
#define  pwmSenseGet(n) pwmSenseValue[n]

// The following is for use by interrupt.c
extern uint16_t pwmStartTime0;
extern uint16_t pwmStartTime1;
extern uint16_t pwmStartTime2;
#define ISR_PWMSENSE { \
    if(INTCONbits.INT0IF) { \
        if(INTCON2bits.INTEDG0) { \
            pwmStartTime0 = TMR0; /* Rising edge. Note the time */ \
            INTCON2bits.INTEDG0 = 0; /* Set to falling edge */ \
        } else { \
            pwmSenseValue[0] = TMR0 - 65 - pwmStartTime0; /* Falling edge. Calculate pulse width. */ \
            INTCON2bits.INTEDG0 = 1; /* Set to rising edge */ \
        } \
        INTCONbits.INT0IF = 0; \
    } \
    if(INTCON3bits.INT1IF) { \
        if(INTCON2bits.INTEDG1) { \
            pwmStartTime1 = TMR0; \
            INTCON2bits.INTEDG1 = 0; \
        } else { \
            pwmSenseValue[1] = TMR0 - 65 - pwmStartTime1; \
            INTCON2bits.INTEDG1 = 1; \
        } \
        INTCON3bits.INT1IF = 0; \
    } \
    if(INTCON3bits.INT2IF) { \
        if(INTCON2bits.INTEDG2) { \
            pwmStartTime2 = TMR0; \
            INTCON2bits.INTEDG2 = 0; \
        } else { \
            pwmSenseValue[2] = TMR0 - 65 - pwmStartTime2; \
            INTCON2bits.INTEDG2 = 1; \
        } \
        INTCON3bits.INT2IF = 0; \
    } \
}

#endif	/* PWMSENSE_H */
