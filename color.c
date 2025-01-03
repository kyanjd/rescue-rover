#include <xc.h>
#include <math.h>
#include "color.h"
#include "i2c.h"

void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
    
    // turning on R, G, B LEDs
    TRISGbits.TRISG1 = 0;
    TRISAbits.TRISA4 = 0;
    TRISFbits.TRISF7 = 0;
    LED_R = 1;
    LED_G = 1;
    LED_B = 1;
}

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Green(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Blue(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Blue LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Blue MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

void color_read(struct RGB_val *rgb){
    rgb->R = color_read_Red(); // reading red value
    rgb->G = color_read_Green(); // reading green value
    rgb->B = color_read_Blue(); // reading blue value
}

unsigned char color_detection(float h, float s, float v){
    unsigned char colour=0; // ambient
    // use h s v values to find out which colour is detected
    if (h > 320 && h < 350 && s > 67){colour = 1;} // red 
    else if (h < 200 && s > 30 && s < 50 && v < 25){colour=2;} // green
    else if (h < 230 && h > 200 && s < 70  && v > 5 && v < 50) {colour=3;} // blue 
    else if (h > 310 && s > 22 && s < 35 && v > 40) {colour=4;} // yellow
    else if (h > 260 && h < 310 && s < 35 && s > 20 && v > 60) {colour=5;} // pink
    else if (h > 330 && s > 55 && s < 67) {colour=6;} // orange
    else if (h > 170 && h < 225 && s > 25 && s < 45 && v > 50) {colour=7;} // pale blue
    else if (h < 260 && h > 220 && s < 40 && v > 65) {colour=8;} // white - wall detected - return back
    else if (h < 250 && s < 30 && v < 5) {colour=8;} // black - wall detected - return back 
    else {colour=0;}    
    return colour;
}

unsigned char RGB_to_HSV(struct RGB_val *rgb){
    
    color_read(rgb);
    // normalising r, g and b values scaled to 0-1
    float red = rgb->R/12100.0f; // 12100 is the highest red value recorded 
    float green = rgb->G/8200.0f; // 8200 is the highest green value recorded
    float blue = rgb->B/4300.0f; // 4300 is the highest blue value recorded
    
    // Find the maximum and minimum RGB values
    float max = (red > green) ? (red > blue ? red : blue) : (green > blue ? green : blue);
    float min = (red < green) ? (red < blue ? red : blue) : (green < blue ? green : blue);
    float delta = max - min;

    // Calculate Hue
    float h = 0.0f;
    if (delta == 0) {h = 0;}
    else if (max == red) {h = fmod(60.0f * ((green - blue) / delta) + 360.0f, 360.0f);} 
    else if (max == green) {h = fmod(60.0f * ((blue - red) / delta) + 120.0f, 360.0f);} 
    else if (max == blue) {h = fmod(60.0f * ((red - green) / delta) + 240.0f, 360.0f);}
    if (h < 0) {h += 360.0f;} // scale between 0-360

    // Calculate Saturation
    float s = ((max == 0) ? 0 : (delta / max)) * 100.0f; //scale between 0-100

    // Calculate Value
    float v = max*100.0f; // scale between 0-100
    return color_detection(h, s, v);
}