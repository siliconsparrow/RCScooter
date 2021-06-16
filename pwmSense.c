// Attempt to use interrupts and timers to measure the pulse width of incoming
// PWM signals (up to four simultaneous channels).

// This will be used to receive signals intended for servo motors. There will be
// a pulse every 20ms and the pulse width will vary between 400us and 2400us.

// I'd really like to use a timer capture for this but I'd need three channels
// and the PIC1824240 only has two - plus I use one of them for motor speed
// control so there's only one left. I'll just use GPIO interrupts and grab
// the time off a free-running timer.

// A timer resolution of 10us should be good.

#include "board.h"
#include "pwmSense.h"

volatile uint8_t pwmSenseValue[PWM_SENSE_CHANNELS] = { 0 };
uint16_t pwmStartTime0 = 0;
uint16_t pwmStartTime1 = 0;
uint16_t pwmStartTime2 = 0;

void pwmSenseInit(void)
{
    // Use Timer0 to measure the pulse widths.
    T0CON = 0b00000010; // Set 16-bit mode, 1:8 prescaler (resolution 8us at CPU=4MHz)
    T0CON |= 0b10000000; // Enable the timer.
    
    // Set pins to input
    SENSE0_DDR = INPUT;
    SENSE1_DDR = INPUT;
    SENSE2_DDR = INPUT;
    
    // Set up GPIO interrupts on pins RB0..RB2
    INTCONbits.INT0IF   = 0; // Clear interrupt flag.
    INTCON2bits.INTEDG0 = 1; // Rising edge.
    INTCONbits.INT0IE   = 1; // Enable interrupt.
    INTCON3bits.INT1IF  = 0;
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1IE  = 1;
    INTCON3bits.INT2IF  = 0;
    INTCON2bits.INTEDG2 = 1;
    INTCON3bits.INT2IE  = 1;
    
    // The rest is handled by the ISR.
}
