#include <xc.h>
#include "calibrate.h"
#include "dc_motor.h"

void Calibrate_init(void)
{
    // setup RD7 pin for output (connected to LED)
    LATDbits.LATD7=0;   //set initial output state (off)
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    // setup RH3 pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state (off)
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    
    // setup RF2 pin for input (connected to button)
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
    
    // setup RF3 pin for input (connected to button)
    TRISFbits.TRISF3=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin
}


void calibrate_angle(DC_motor *mL, DC_motor *mR, Calibration_constants *c)
{
    __delay_ms(1000);
    while (1) { //continuous large calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->angle_constant += 20; //increase how much buggy turns
                if (c->angle_constant >= 1000) {c->angle_constant = 1000;} //upper threshold
                turn_right(mL, mR, 180, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->angle_constant -= 20; //decrease how much buggy turns
                if (c->angle_constant <= 0 || c->angle_constant >= 1000) {c->angle_constant = 0;} //lower threshold accounting for underflow
                turn_right(mL, mR, 180, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of large calibration
    LATDbits.LATD7 = 0;
    __delay_ms(150);
    
    while (1) { //continuous fine calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->angle_constant += 2; //increase how much buggy turns
                if (c->angle_constant >= 1000) {c->angle_constant = 1000;} //upper threshold
                turn_right(mL, mR, 180, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->angle_constant -= 2; //decrease how much buggy turns
                if (c->angle_constant <= 0 || c->angle_constant >= 1000) {c->angle_constant = 0;} //lower threshold accounting for underflow
                turn_right(mL, mR, 180, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of calibrating
    LATDbits.LATD7 = 0;
}

void calibrate_distance(DC_motor *mL, DC_motor *mR, Calibration_constants *c)
{
    __delay_ms(1000);
    while (1) { //continuous large calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->one_tile += 100; //increase how far one tile is
                if (c->one_tile >= 3000) {c->one_tile = 3000;} //upper threshold
                full_speed(mL, mR, 1, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->one_tile -= 100; //decrease how far one tile is
                if (c->one_tile <= 50 || c->one_tile >= 2000) {c->one_tile = 50;} //lower threshold accounting for underflow
                full_speed(mL, mR, 1, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of large calibration
    LATDbits.LATD7 = 0;
    __delay_ms(150);
    
    while (1) { //continuous fine calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->one_tile += 10; //increase how far one tile is
                if (c->one_tile >= 3000) {c->one_tile = 3000;} //upper threshold
                full_speed(mL, mR, 1, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->one_tile -= 10; //decrease how far one tile is
                if (c->one_tile <= 50 || c->one_tile >= 2000) {c->one_tile = 50;} //lower threshold accounting for underflow
                full_speed(mL, mR, 1, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of calibrating
    LATDbits.LATD7 = 0;
}

void calibrate_half(DC_motor *mL, DC_motor *mR, Calibration_constants *c)
{
    __delay_ms(1000);
    while (1) { //continuous large calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->half_tile += 100; //increase far half a tile is
                if (c->half_tile >= 3000) {c->half_tile = 3000;} //upper threshold
                full_speed_half(mL, mR, 1, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->half_tile -= 100; //decrease how far half a tile is
                if (c->half_tile <= 50 || c->half_tile >= 2000) {c->half_tile = 50;} //lower threshold accounting for underflow
                full_speed_half(mL, mR, 1, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of large calibration
    LATDbits.LATD7 = 0;
    __delay_ms(150);
    
    while (1) { //continuous fine calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 50ms
                else {
                __delay_ms(250);
                c->half_tile += 10; //increase how far half a tile is
                if (c->half_tile >= 3000) {c->half_tile = 3000;} //upper threshold
                full_speed_half(mL, mR, 1, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 50ms
                else {
                __delay_ms(250);
                c->half_tile -= 10; //decrease how far half a tile is
                if (c->half_tile <= 50 || c->half_tile >= 2000) {c->half_tile = 50;} //lower threshold accounting for underflow
                full_speed_half(mL, mR, 1, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of calibrating
    LATDbits.LATD7 = 0;
}

void calibrate_imbalance(DC_motor *mL, DC_motor *mR, Calibration_constants *c)
{
    __delay_ms(1000);
    while (1) { //continuous calibration
        LATHbits.LATH3 = 1; //LEDs on to signify calibrating
        LATDbits.LATD7 = 1;
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
            if (!PORTFbits.RF2) {__delay_ms(100); //right button logic
                if (!PORTFbits.RF3) {__delay_ms(500); break;} //break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->motor_imbalance += 1; //increase the extra power applied to the left motor (under-performs from testing)
                if (c->motor_imbalance >= 10) {c->motor_imbalance = 10;} //upper threshold
                full_speed(mL, mR, 1, c);
            } 
            }
            if (!PORTFbits.RF3) {__delay_ms(100); //left button logic
                if (!PORTFbits.RF2) {__delay_ms(500); break;}//break if both are pressed within 100ms
                else {
                __delay_ms(250);
                c->motor_imbalance -= 1; //decrease the extra power applied to the left motor
                if (c->motor_imbalance <= 0 || c->motor_imbalance >= 10) {c->motor_imbalance = 0;} //lower threshold accounting for underflow
                full_speed(mL, mR, 1, c);
            }
            }       
        }
    __delay_ms(150);
    LATHbits.LATH3 = 0; //LEDs off to signify end of calibrating
    LATDbits.LATD7 = 0;
}