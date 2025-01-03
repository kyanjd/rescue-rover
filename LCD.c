#include <xc.h>
#include "LCD.h"
#include <stdio.h>

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on
    LCD_E = 1;
	__delay_us(2); //wait a short delay
	//turn the LCD enable bit off again
    LCD_E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{
	//set the data lines here (think back to LED array output)
    LCD_DB7 = number >> 3 & 0b1; // DB7
    LCD_DB6 = number >> 2 & 0b1; // DB6
    LCD_DB5 = number >> 1 & 0b1; // DB5
    LCD_DB4 = number & 0b1; // DB4
        
    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);      //Delay 5uS
}


/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    LCD_RS = type; // RS
    
    // send high bits of Byte using LCDout function
    LCD_sendnibble(Byte >> 4);
    
    // send low bits of Byte using LCDout function
    LCD_sendnibble(Byte & 0b00001111);	
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{
    //Define LCD Pins as Outputs and
    //set all pins low (might be random values on start up, fixes lots of issues)
    TRISEbits.TRISE1 = 0; // DB7
    TRISEbits.TRISE3 = 0; // DB6
    TRISBbits.TRISB2 = 0; // DB5
    TRISBbits.TRISB3 = 0; // DB4
    TRISCbits.TRISC2 = 0; // E
    TRISCbits.TRISC6 = 0; // RS
    
    LCD_RS = 0;
    LCD_E = 0;
    LCD_DB4 = 0;
    LCD_DB5 = 0;
    LCD_DB6 = 0;
    LCD_DB7 = 0;
    
    __delay_ms(100);
    
    // Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
	// after this use LCD_sendbyte to operate in 4 bit mode
    
    LCD_sendnibble(0x3);
    __delay_us(40);
    
    LCD_sendbyte(0x2C, 0);
    __delay_us(40);
    
    LCD_sendbyte(0x2C, 0);
    __delay_us(38);
    
    LCD_sendbyte(0x0C, 0);
    __delay_us(38);
    
    LCD_sendbyte(0x1, 0);
    __delay_ms(2);
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
    LCD_sendbyte(0x6, 0);
    __delay_us(53);
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if (line==0){LCD_sendbyte(0x80,  0);}
    //Send 0xC0 to set line to 2 (0x40 ddram address)
    else if (line==1) {LCD_sendbyte(0xC0, 0);}
    __delay_us(50);
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{
	//code here to send a string to LCD using pointers and LCD_sendbyte function
    while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		LCD_sendbyte(*string++,1); 	//Send out the current byte pointed to and increment the pointer
	}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
	//code here to scroll the text on the LCD screen
    LCD_sendbyte(0b00011000, 0);
    __delay_us(39);
}


void LCD_SetCGRAM(unsigned char address, unsigned char *pattern) {
    LCD_sendbyte(0x40 | (address << 3), 0);
    for (int i = 0; i < 8; i++){
        LCD_sendbyte(pattern[i], 1);
    }
}

/************************************
 * Function takes a ADC value and works out the voltage to 2 dp
 * the result is stored in buf as ascii text ready for display on LCD
 * Note result is stored in a buffer using pointers, it is not sent to the LCD
************************************/
void ADC2String(char *buf, unsigned int ADC_val){
	//code to calculate the inegeter and fractions part of a ADC value
	// and format as a string using sprintf (see GitHub readme)
    unsigned int ADC_result = ADC_val;
    unsigned int int_part=ADC_result/78; // i.e. 255 / 5V = 51
    unsigned int frac_part=(ADC_result*100)/78 - int_part*100;
    sprintf(buf,"%d.%02d",int_part,frac_part);
    
}
