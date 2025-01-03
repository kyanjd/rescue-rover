#include <xc.h>
#include "dc_motor.h"
#include "stack.h"
#include "color.h"
#include "head_lights.h"
#include "timers.h"

void DCmotors_PWM_init(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2 = 0; // Set RE2 as output for CCP1
    TRISEbits.TRISE4 = 0; // Set RE4 as output for CCP2
    TRISCbits.TRISC7 = 0; // Set RC7 as output for CCP3
    TRISGbits.TRISG6 = 0; // Set RG6 as output for CCP4
    LATEbits.LATE2 = 0;   // Initialise RE2 output low
    LATEbits.LATE4 = 0;   // Initialise RE4 output low
    LATCbits.LATC7 = 0;   // Initialise RC7 output low
    LATGbits.LATG6 = 0;   // Initialise RG6 output low
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=0b100; //1:16
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod;
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; // if set to T2PR or higher this is 100% duty cycle 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
}

void set_motor_PWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=m->PWMperiod; //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

void change_motor_speed(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int target_power, unsigned int step)
{
    if (GLOBAL_FAST_MODE) { //fast
        if (target_power > mL->power && target_power > mR->power) { // increment both motor powers by step every 500us then set to target with imbalance
            for (int i = mL->power; i <= target_power-step; i+=step) {
                mL->power = i;
                mR->power = i;
                set_motor_PWM(mL);
                set_motor_PWM(mR); 
                __delay_us(500);
            }
            if (target_power + c->motor_imbalance > 100) {mL->power = target_power;} 
            else {mL->power = target_power + c->motor_imbalance;}
            mR->power = target_power;
            set_motor_PWM(mL);
            set_motor_PWM(mR);  
        }
        else if (target_power < mL->power && target_power < mR->power) { //decrement both motor powers by step every 500us then set to target
            for (int i = mL->power; i >= target_power+step; i-=step) {
                mL->power = i;
                mR->power = i;
                set_motor_PWM(mL);
                set_motor_PWM(mR);    
                __delay_us(500);
            }
            if (target_power + c->motor_imbalance > 100) {mL->power = target_power;}
            else {mL->power = target_power + c->motor_imbalance;}
            mR->power = target_power;
            set_motor_PWM(mL);
            set_motor_PWM(mR);  
        } 
    }
    else { //slow
        if (target_power > mL->power && target_power > mR->power) { //same functionality in slow except apply increment every 1 ms instead
            for (int i = mL->power; i <= target_power-step; i+=step) {
                mL->power = i;
                mR->power = i;
                set_motor_PWM(mL);
                set_motor_PWM(mR); 
                __delay_ms(1);
            }
            if (target_power + c->motor_imbalance > 100) {mL->power = target_power;}
            else {mL->power = target_power + c->motor_imbalance;}
            mR->power = target_power;
            set_motor_PWM(mL);
            set_motor_PWM(mR);  
        }
        else if (target_power < mL->power && target_power < mR->power) {
            for (int i = mL->power; i >= target_power+step; i-=step) {
                mL->power = i;
                mR->power = i;
                set_motor_PWM(mL);
                set_motor_PWM(mR);    
                __delay_ms(1);
            }
            if (target_power + c->motor_imbalance > 100) {mL->power = target_power;}
            else {mL->power = target_power + c->motor_imbalance;}
            mR->power = target_power;
            set_motor_PWM(mL);
            set_motor_PWM(mR);  
        }
    }
}

void stop(DC_motor *mL, DC_motor *mR, Calibration_constants *c)
{
    brake_on(); //lights
    change_motor_speed(mL, mR, 0, 50, c); //change speed to 0 with 50 increment
}

void turn_left(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int angle)
{
    stop(mL,mR, c);
    mL->direction = 0; // Reverse left motor
    mR->direction = 1; // Forward right motor
    
    for (int i=0; i<angle/45; i++) { // e.g. do twice for 90
        
        if (GLOBAL_FAST_MODE) {
            change_motor_speed(mL, mR, 50, 10, c); // larger increment = faster 
        }
        else {
            change_motor_speed(mL, mR, 50, 5, c);
        }

        unsigned int delay_time = (45 * c->angle_constant) / 100;
        VariableDelay(delay_time); // function to allow __delay_ms to take a variable input
        stop(mL,mR, c);
        turn_left_light_on();
        __delay_ms(300); // let buggy settle before doing next movement - larger leads to better turns from testing
    }
}

void turn_right(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int angle)
{
    stop(mL,mR, c);
    mL->direction = 1; // Reverse left motor
    mR->direction = 0; // Forward right motor
    
    for (int i=0; i<angle/45; i++) {
        
        if (GLOBAL_FAST_MODE) {
            change_motor_speed(mL, mR, 50, 10, c); // larger increment = faster
        }
        else {
            change_motor_speed(mL, mR, 50, 5, c);
        }

        unsigned int delay_time = (45 * c->angle_constant) / 100;
        VariableDelay(delay_time); // function to allow __delay_ms to take a variable input
        stop(mL,mR, c);
        turn_right_light_on();
        __delay_ms(300); // let buggy settle before doing next movement - larger leads to better turns from testing
    }
}

void full_speed(DC_motor *mL, DC_motor *mR, Calibration_constants *c, char direction)
{
    main_beam_on();
    stop(mL, mR, c);
    mL->direction = direction;
    mR->direction = direction;
    
    if (GLOBAL_FAST_MODE) {
        change_motor_speed(mL, mR, 40, 10, c);  // higher top speed
    }
    else {
        change_motor_speed(mL, mR, 30, 10, c);  
    }
    

    VariableDelay(c->one_tile); // from calibration
    stop(mL, mR, c);
}

void full_speed_half(DC_motor *mL, DC_motor *mR, Calibration_constants *c, char direction)
{
    main_beam_on();
    stop(mL, mR, c);
    mL->direction = direction; // 1 for forward, 0 for backward
    mR->direction = direction;
    
    if (GLOBAL_FAST_MODE) {
        change_motor_speed(mL, mR, 40, 10, c); // higher top speed
    }
    else {
        change_motor_speed(mL, mR, 30, 10, c);  
    }
    VariableDelay(c->half_tile); // from calibration
    stop(mL, mR, c);
}

void max_speed(DC_motor *mL, DC_motor *mR ,Calibration_constants *c)
{

    mL->direction = 1;
    mR->direction = 1;
    
    change_motor_speed(mL, mR, 70, 10, c); // fast into wall for alignment

    __delay_ms(200); // no variable delay needed - same no matter what
    stop(mL, mR, c);
}

unsigned char white_instruction(DC_motor *mL, DC_motor *mR, Calibration_constants *c, Stack *s)
{
    LED_R = 0; // lights off
    LED_G = 0;
    LED_B = 0;
    unsigned char colour_zero_count = 0;
    while (1) {
        unsigned char colour = stack_pop(s); //get and then remove the top colour off the stack
        if (colour == 10) { //10 represents empty stack so turn 720 and end program
            turn_right(mL, mR, 720, c);
            celebration_lights();
            colour_zero_count = 0; // reset for next run
            Stack_init(s); // reset for next run
            return 0; // tells main.c that program is finished
        }
        
        if (colour == 8) {continue;} //skip half steps in fast mode
        if (colour == 0 && colour_zero_count == 0) { //ignore the first instance of recorded 0 followed by another 0 to improve return efficiency
            colour_zero_count ++;
        }

        else if (colour == 0 && colour_zero_count > 0) { //move back 1 tile for all subsequent recorded 0
            full_speed(mL, mR, 0, c);
            __delay_ms(100);
        }
        else if (colour != 0) {
            if (GLOBAL_FAST_MODE) {colour_zero_count = 0;} //reset 0 counter in fast mode as every first 0 should be ignored (not used in SLOW_MODE)
            switch (colour) { //carry out reverse movement instructions
                case 1: turn_left(mL, mR, 90, c); break; // red backwards = green
                case 2: turn_right(mL, mR, 90, c); break; // green backwards = red
                case 3: turn_right(mL, mR, 180, c); break; // blue backwards = blue
                case 4: turn_left(mL, mR, 90, c); __delay_ms(100); full_speed(mL, mR, 1, c); break; // backwards yellow
                case 5: turn_right(mL, mR, 90, c); __delay_ms(100); full_speed(mL, mR, 1, c); break; // backwards pink
                case 6: turn_left(mL, mR, 135, c); break; // orange backwards = light blue
                case 7: turn_right(mL, mR, 135, c); break; // light blue backwards = orange
            } 
            __delay_ms(100);
            if (!GLOBAL_FAST_MODE) { // SLOW_MODE includes wall bounce/alignment
                __delay_ms(100);
                full_speed_half(mL, mR, 0, c);
                colour_zero_count = 0;
                __delay_ms(100);
            }
        }          
    } 
    __delay_ms(100); // settling delay
    return 1; // tell main.c program is still running
}  

unsigned char colour_instructions(DC_motor *mL, DC_motor *mR, Calibration_constants *c, Stack *s, unsigned char colour)
{
    stack_push(s, colour); // add current colour to stack and make new top
    if (colour == 0) { //go forwards 1 tile when no colour is detected
        LED_R = 1; // lights on
        LED_G = 1; 
        LED_B = 1; 
        full_speed(mL, mR, 1, c); 
        __delay_ms(100);
    } 
    else { //colour detected meaning buggy is pressed against wall
        LED_R = 0; // lights off
        LED_G = 0;
        LED_B = 0;
        brake_on();
        max_speed(mL, mR, c); // align on wall
        __delay_ms(100);
        full_speed_half(mL, mR, 0, c); //back up to middle of previous square before performing colour action
        __delay_ms(200);
        switch (colour) {
            case 1: turn_right(mL, mR, 90, c); break; // red
            case 2: turn_left(mL, mR, 90, c); break; // green
            case 3: turn_right(mL, mR, 180, c); break; // blue
            case 4: full_speed(mL, mR, 0, c); __delay_ms(200); turn_right(mL, mR, 90, c); break; // yellow
            case 5: full_speed(mL, mR, 0, c); __delay_ms(200); turn_left(mL, mR, 90, c); break; // pink
            case 6: turn_right(mL, mR, 135, c); break; // orange
            case 7: turn_left(mL, mR, 135, c); break; // light blue
            case 8: __delay_ms(500); return white_instruction(mL, mR, c, s); // pause then run white_instruction and return its return (1 if running, 0 if finished)
        }
        LED_R = 1; // lights back on
        LED_G = 1;
        LED_B = 1;
        
    }
    __delay_ms(100);
    return 1; // tell main.c program is still running
}
