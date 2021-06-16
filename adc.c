// *************************************************************************
// **
// ** Analogue to Digital functions for the PIC18F24K20
// **
// **   by Adam Pierce <adam@siliconsparrow.com>
// **   for Levaux Pty. Ltd.
// **   created 13-Feb-2014
// **
// *************************************************************************

#include "board.h"
#include "adc.h"

void adcInit(void)
{
	// Activate ADC channels.
	// Voltage reference is VDD/VSS.
	// Acquisition set for slower but good accuracy.
	ADCON0 = 0x00;
	ADCON1 = ADC_MASK;
	ADCON2 = 0b10111001; // Tad = 2µs, Tacq = 20Tad
						 // Total sample time = 66µs

	// Enable the 1.2V on-chip reference voltage.
	//CVRCON2bits.FVREN = 1;

	// Enable the ADC.
	ADCON0bits.ADON = 1;
}

// Shuts down ADC to save power.
void adcDisable(void)
{
	//CVRCON2bits.FVREN = 0;
	ADCON0bits.ADON = 0;
}

// Read the ADC
uint16_t adcRead(uint8_t channel)
{
    ADCON0 = 1 | ((channel) << 2);
	ADCON0bits.GO = 1;
    while(0 != ADCON0bits.GO)
        ;
    
    return ADRES;
}
