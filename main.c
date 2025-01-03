// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "dc_motor.h"
#include "timers.h"
#include "calibrate.h"
#include "stack.h"
#include "head_lights.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    // necessary inits
    Timer0_init();
    Calibrate_init();
    color_click_init();
    head_lights_init();
    
    
    // setup left and right motor structures
    unsigned int PWMcycle = 99;
    DCmotors_PWM_init(PWMcycle);
    struct DC_motor motorL, motorR; 		//declare two DC_motor structures 
    
    motorL.power=0; 						//zero power to start
    motorL.direction=1; 					//set default motor direction
    motorL.brakemode=1;						// brake mode (slow decay)
    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
    motorL.PWMperiod=PWMcycle; 			//store PWMperiod for motor (value of T2PR in this case)
    
    motorR.power=0; 						//zero power to start
    motorR.direction=1; 					//set default motor direction
    motorR.brakemode=1;						// brake mode (slow decay)
    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
    motorR.PWMperiod=PWMcycle; 			//store PWMperiod for motor (value of T2PR in this case)
    
    // setup RGB structure
    struct RGB_val rgb;
    
    // setup stack and initiation
    struct Stack movements;
    Stack_init(&movements);
    
    // setup structure to store calibration constants
    struct Calibration_constants cal_const = {320, 1400, 600, 4}; //{angle_constant, one_tile, half_tile, motor_imbalance}, saved
    
    // calibrate motor imbalance
    calibrate_imbalance(&motorL, &motorR, &cal_const);
    
    // calibrate buggy turning 
    calibrate_angle(&motorL, &motorR, &cal_const);
    
    // calibrate buggy straight line distance
    calibrate_distance(&motorL, &motorR, &cal_const);
    
    // calibrate buggy half distance
    calibrate_half(&motorL, &motorR, &cal_const);
    
    while (1) {
        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop to wait for button press
        if (!PORTFbits.RF3) {GLOBAL_FAST_MODE = 0;} //left button starts slow run
        else if (!PORTFbits.RF2) {GLOBAL_FAST_MODE = 1;} //right button starts fast run
        
        // small delay before navigating maze
        __delay_ms(1000);

        while (1) {
            char colour = RGB_to_HSV(&rgb);
            if(!colour_instructions(&motorL, &motorR, &cal_const, colour, &movements)) { //reset to before mode selection if 0 returned meaning end of run
                break;
            }
        }
    }
}
