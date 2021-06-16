// ***************************************************************************
// **
// ** Board definitions for RCScooter
// **
// **   for the PIC182420
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   7-Jun-2021
// **
// ***************************************************************************

#ifndef _BOARD_H_
#define _BOARD_H_

#include <xc.h> // Chip definitions.

/* Pinout of PIC18F2420
   --------------------
    (A=ADC I=IN O=OUT P=PWM B=BIDIRECTIONAL Q=IRQ)
  
	RA0:AN0  A Left motor current sense
	RA1:AN1  A Right motor current sense
	RA2:AN2  - <unused> (future position sense for steering)
	RA3      - <unused>
	RA4      O Steering actuator forward switch
	RA5      O Steering actuator reverse switch
	RA6      O Aux actuator forward switch
	RA7      O Aux actuator reverse switch
	
	RB0      Q RC PWM Sense channel 1
	RB1      Q RC PWM Sense channel 2
	RB2      Q RC PWM Sense channel 3
	RB3      - <unused>
	RB4      - <unused>
	RB5      - <unused>
	RB6      I In-circuit programming clock
	RB7      B In-circuit programming data
	
	RC0      O Test LED (1=on)
	RC1:CCP2 O PWM for main motor control
	RC2:CCP1 O PWM for steering and aux motors
	RC3      - <unused>
	RC4      - <unused>
	RC5      - <unused>
	RC6:TX   O UART transmit for testing
	RC7:RX   I UART receive for testing
	
	RE3      I VPP/Reset
*/

// GPIO definitions.
#define LED0          LATCbits.LATC0
#define LED0_DDR      TRISCbits.TRISC0
#define MOT1_CTL1     LATAbits.LATA4
#define MOT1_CTL1_DDR TRISAbits.TRISA4
#define MOT1_CTL2     LATAbits.LATA5
#define MOT1_CTL2_DDR TRISAbits.TRISA5
#define MOT2_CTL1     LATAbits.LATA6
#define MOT2_CTL1_DDR TRISAbits.TRISA6
#define MOT2_CTL2     LATAbits.LATA7
#define MOT2_CTL2_DDR TRISAbits.TRISA7
#define SENSE0_DDR    TRISBbits.TRISB0
#define SENSE1_DDR    TRISBbits.TRISB1
#define SENSE2_DDR    TRISBbits.TRISB2

// PWM pins.
#define PWM1_OUT_DDR  DDRCbits.TRISC2
#define PWM2_OUT_DDR  DDRCbits.TRISC1

// ADC channels
#define ADC_CHAN_STEER     0 // Current sense for steering actuator (1V=1A)
#define ADC_CHAN_AUX       1 // Current sense for aux motor control (1V=1A)
#define ADC_CHAN_STEER_POS 2 // Future position sense for the steering

#define ADC_MASK 0x0C // ADC port configuration (see ADCON1/PCFG table in the manual)
                      // 0x0C -> ADC channels 0, 1 & 2 are enabled.

// CPU Clock settings.
#define OSC_EXT    0x00 // Use external crystal oscillator.
#define OSC_TIM1   0x01 // Use Timer1 as CPU oscillator.
#define OSC_INT    0x02 // Use internal oscillator.
#define OSC_IDLEN  0x80 // Use IDLE mode instead of SLEEP.
#define OSC_8MHZ   0x70 // Internal oscillator set to 8MHz
#define OSC_4MHZ   0x60 // Internal oscillator set to 4MHz
#define OSC_2MHZ   0x50 // Internal oscillator set to 2MHz
#define OSC_1MHZ   0x40 // Internal oscillator set to 1MHz
#define OSC_500KHZ 0x30 // Internal oscillator set to 500kHz
#define OSC_250KHZ 0x20 // Internal oscillator set to 250kHz
#define OSC_125KHZ 0x10 // Internal oscillator set to 125kHz
#define OSC_31KHZ  0x00 // Internal oscillator set to 31.25kHz

#define OSCCON_SETTING (OSC_INT|OSC_4MHZ)
#define CPU_FOSC       4000000

// Interrupt control.
#define INTERRUPTS_ENABLE  INTCONbits.GIE=1
#define INTERRUPTS_DISABLE INTCONbits.GIE=0

#define OUTPUT 0
#define INPUT  1

void boardInit(void);

#endif // _BOARD_H_
