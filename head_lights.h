#ifndef HEAD_LIGHTS_H
#define	HEAD_LIGHTS_H

#define MAIN_BEAM LATDbits.LATD3
#define BRAKE LATDbits.LATD4
#define TURN_LEFT LATAbits.LATA5
#define TURN_RIGHT LATHbits.LATH0
#define _XTAL_FREQ 64000000


/********************************************//**
 *  Function to initiate the head lights on the buggy
 ***********************************************/
void head_lights_init(void);

/********************************************//**
 *  Function to turn on the main beam and turn off the brake lights
 ***********************************************/
void main_beam_on(void);

/********************************************//**
 *  Function to flicker the left indicator light
 ***********************************************/
void turn_left_light_on(void);

/********************************************//**
 *  Function to flicker the right indicator light
 ***********************************************/
void turn_right_light_on(void);

/********************************************//**
 *  Function to turn on the brake lights and turn off the main beam
 ***********************************************/
void brake_on(void);

/********************************************//**
 *  Function to do a celebration using all the head lights
 ***********************************************/
void celebration_lights(void);

#endif	/* HEAD_LIGHTS_H */

