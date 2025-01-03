#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // 1:256 so overflow occurs each second (65536 / 2^8) (perfect is ~244)
    T0CON0bits.T016BIT=1;	//16bit mode	
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=11;            // change initial value to account for slightly off prescaler division (256-244) * 2^8
    TMR0L=219;
    T0CON0bits.T0EN=1;	//start the timer
}

// Function to create a delay of length "delay_time" milliseconds
void VariableDelay(unsigned int delay_time){
    for (int i=0; i<delay_time; i++){
        __delay_us(200);
    }
}

/************************************
 * Function to return the full 16bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
************************************/
unsigned int get16bitTMR0val(void)
{
	//add your code to get the full 16 bit timer value here
    unsigned int Lbitcount = TMR0L; // read low reg because necessary
    unsigned int Hbitcount = TMR0H; // read high reg for return
    
    return Hbitcount;
}
