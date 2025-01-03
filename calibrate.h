#ifndef _calibrate_H
#define _calibrate_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/********************************************//**
 *  Function to initiate the calibration
 ***********************************************/
void Calibrate_init(void);

/********************************************//**
 *  Function to calibrate the angle of a buggy turn
***********************************************/ 
void calibrate_angle(DC_motor *mL, DC_motor *mR, Calibration_constants *c);

/********************************************//**
 *  Function to calibrate the distance of one tile for the buggy
 ***********************************************/
void calibrate_distance(DC_motor *mL, DC_motor *mR, Calibration_constants *c);

/********************************************//**
 *  Function to calibrate distance of half a tile
 ***********************************************/
void calibrate_half(DC_motor *mL, DC_motor *mR, Calibration_constants *c);

/********************************************//**
 *  Function to calibrate left and right motor power to avoid drifting
 ***********************************************/
void calibrate_imbalance(DC_motor *mL, DC_motor *mR, Calibration_constants *c);
#endif
