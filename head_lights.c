#include <xc.h>
#include "head_lights.h"

void head_lights_init(void){
    TRISDbits.TRISD3 = 0; // main beam tris
    MAIN_BEAM = 0;
    TRISDbits.TRISD4 = 0; // brake tris
    BRAKE = 0; 
    TRISAbits.TRISA5 = 0; // turn left tris
    TURN_LEFT = 0;
    TRISHbits.TRISH0 = 0; // turn right tris
    TURN_RIGHT = 0;
}

void main_beam_on(void){
    MAIN_BEAM = 1;
    BRAKE = 0;
}

void turn_left_light_on(void){
    TURN_LEFT = !TURN_LEFT; // toggles indicator
}

void turn_right_light_on(void){
    TURN_RIGHT = !TURN_RIGHT; // toggles indicator
}

void brake_on(void){
    BRAKE = 1;
    MAIN_BEAM = 0;
}

void celebration_lights(void){
    for (int i=0; i<10; i++){ // toggles all lights 
        TURN_RIGHT = !TURN_RIGHT;
        __delay_ms(100);
        TURN_LEFT = !TURN_LEFT;
        __delay_ms(100);
        MAIN_BEAM = !MAIN_BEAM;
    }
}