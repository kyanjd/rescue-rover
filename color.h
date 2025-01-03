#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
#define LED_R LATGbits.LATG1
#define LED_G LATAbits.LATA4
#define LED_B LATFbits.LATF7

#define RED 0b1
#define BLUE 0b10
#define GREEN 0b11
#define YELLOW 0b100
#define ORANGE 0b101
#define PALE_BLUE 0b110
#define WHITE 0b111
#define BLACK 0b1000

#define COLOR_THRESHOLD 1050

/********************************************//**
 *  Struct to point to RGB values read
 ***********************************************/
struct RGB_val { 
	unsigned int R;
	unsigned int G;
	unsigned int B;
};


/********************************************//**
 *  Function to setup colour click
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);

/********************************************//**
 *  Function to read R, G, B values
 ***********************************************/
void color_read(struct RGB_val *rgb);

/********************************************//**
 *  Function to covert R, G, B values to HSV values
 ***********************************************/
unsigned char RGB_to_HSV(struct RGB_val *rgb);

/********************************************//**
 *  Function to detect colour from HSV values
 ***********************************************/
unsigned char color_detection(float h, float s, float v);

/********************************************//**
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);

/********************************************//**
 *  Function to read the green channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Green(void);

/********************************************//**
 *  Function to read the blue channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Blue(void);


#endif
