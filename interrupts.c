#include <xc.h>
#include "interrupts.h"
#include "serial.h"

void Interrupts_init(void)
{
    // data receive interrupt
    PIE4bits.RC4IE = 1;
    PIR4bits.RC4IF = 0;
    IPR4bits.RC4IP = 1;
    // data tramission interrupt
    PIE4bits.TX4IE = 0;
    PIR4bits.TX4IF = 0;
    IPR4bits.TX4IP = 1;
    
    // global interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1; 
    
}

void __interrupt(high_priority) HighISR()
{
	if(PIR4bits.TX4IF == 1){
        TX4REG = getCharFromTxBuf();
        if (isDataInTxBuf() == 0){
            PIE4bits.TX4IE = 0;
        }
    }
    if (PIR4bits.RC4IF == 1){
        putCharToRxBuf(RC4REG);
        PIR4bits.RC4IF = 0;
    }    
}

