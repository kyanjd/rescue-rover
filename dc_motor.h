#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "stack.h"

#define _XTAL_FREQ 64000000
/********************************************//**
 *  Global variable to dictate mode
 *  FAST_ MODE is when there is efficient pathing and faster motor speeds
 *  SLOW_MODE uses the wall the self-align itself going back through the maze 
 * and uses slower motor speeds
 ***********************************************/
extern volatile unsigned char GLOBAL_FAST_MODE = 1;

/********************************************//**
 *  Structure to point to various parameters for motor control
 ***********************************************/
typedef struct DC_motor {
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

/********************************************//**
 *  Structure to point to calibration constants used in calibration
 ***********************************************/
typedef struct Calibration_constants {
    unsigned int angle_constant;
    unsigned int one_tile;
    unsigned int half_tile;
    unsigned int motor_imbalance;
} Calibration_constants;

/********************************************//**
 *  Function to initiate the dc_motor attributes
 ***********************************************/
void DCmotors_PWM_init(unsigned int PWMperiod); // function to setup PWM

/********************************************//**
 *  Function to set the motor speeds
 ***********************************************/
void set_motor_PWM(DC_motor *m);

/********************************************//**
 *  Function to change motor speeds for smooth acceleration and deceleration
 ***********************************************/
void change_motor_speed(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int target_power, unsigned int step);

/********************************************//**
 *  Function to stop the motor
 ***********************************************/
void stop(DC_motor *mL, DC_motor *mR, Calibration_constants *c);

/********************************************//**
 *  Function to turn the buggy left by an angle specified
 ***********************************************/
void turn_left(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int angle);

/********************************************//**
 *  Function to turn the buggy right by an angle specified
 ***********************************************/
void turn_right(DC_motor *mL, DC_motor *mR, Calibration_constants *c, unsigned int angle);

/********************************************//**
 *  Function to move the buggy half a tile
 ***********************************************/
void full_speed_half(DC_motor *mL, DC_motor *mR, Calibration_constants *c, char direction);

/********************************************//**
 *  Function to move the buggy a full tile
 ***********************************************/
void full_speed(DC_motor *mL, DC_motor *mR, Calibration_constants *c, char direction);

/********************************************//**
 *  Function to align the buggy with the wall
 ***********************************************/
void max_speed(DC_motor *mL, DC_motor *mR, Calibration_constants *c);

/********************************************//**
 *  Function to start a reverse sequence leading the buggy back to the start
 ***********************************************/
unsigned char white_instruction(DC_motor *mL, DC_motor *mR, Calibration_constants *c, Stack *s);

/********************************************//**
 *  Function to decide which action to do based on the colour reading
 ***********************************************/
unsigned char colour_instructions(DC_motor *mL, DC_motor *mR, Calibration_constants *c, Stack *s, unsigned char colour);

#endif
