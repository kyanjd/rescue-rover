# Rescue-Rover - An autonomous, colour-detecting, route-retracing mine navigation search and rescue buggy hardware and software
**Note:** This repository is a clone of the original repository, with all hardware-related files (e.g., Makefiles, MPLab project files) removed along with the commit history.

## Contents
1. Objectives
2. Mine environment
3. User information
4. Program explanation
5. Demonstration videos
6. Program framework
- 6.1 main.c
- 6.2 color.c
- 6.3 dc_motor.c
- 6.4 stack.c
- 6.5 calibrate.c


## 1. Objectives

Develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  The robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

## 2. "Mine" environment specification

A "mine" is constructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses can require 10 or moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, the buggy must return to the starting position. Nothing about the "mine" can be known to the buggy in advance. An example course to navigate is shown below.

![Navi Diagram](gifs/maze.gif)

## 3. User Information
The buggy navigates through the maze by mapping it into a grid of 'tiles' where one tile is a distance defined by the user in calibration. (See diagram below)
In order to use the buggy to navigate through the maze, please follow these instructions:
1. Place the buggy in the centre of the start 'tile' and wait for two red LEDs to light up (RD7 and RH3)
2. Calibrate buggy imbalance with left and right buttons as defined in [calibrate.c](###-6.5-calibrate.c) 
3. Once satified with the turning press both buttons together and wait for the two red LEDs to light up (RD7 and RH3)
4. Calibrate turning by press the left button (RF3) and the right button (RF2) to increase or decrease the amount it turns to reach 180 degrees
5. Once satified with the turning press both buttons together and wait for the two red LEDs to light up (RD7 and RH3)
6. Repeat for fine tuning of the turning then press both buttons together again
7. To calibrate the distance of one 'tile',  press the left button (RF3) and the right button (RF2) to increase or decrease the distance it travels ensuring it is one tile.
8. Once satified with the turning press both buttons together and wait for the two red LEDs to light up (RD7 and RH3)
9. Repeat for fine tuning of the one tile distance then press both buttons again
10. To calibrate the distance of half a 'tile',  press the left button (RF3) and the right button (RF2) to increase or decrease the distance it travels ensuring it is half a tile.
11. Once satified with the turning press both buttons together and wait for the two red LEDs to light up (RD7 and RH3).
12. Repeat for fine tuning of the half tile distance then press both buttons again
13. Then place the buggy in the centre of the start tile and select the operation mode by selecting the left button (RF3) for SLOW_MODE, which is when the motor speeds are slower and it uses the wall for self-alignment when reversing back to the start, and the right button (RF2) for FAST_MODE, which defines when the buggy operates at higher motor speeds and the wall isn't used for self-alignment.
14. Clicking one of these buttons will confirm calibration is complete, and it will start the navigation after a small delay.


## 4. Program Explanation
### 4.1 Colour detection
In order to detect and identify colours, RED, GREEN and BLUE values are read using the colour click board with I2C communication. These values are converted to Hue (colour), Saturation (colour depth) and Value (brightness) numbers as they specifiy colour in a more intuitive mannner and is better for colour dectection as it is invariant to illumination changes and robust  to  any conditions [(Performance Comparison Between RGB and HSV Color Segmentations)](https://www.researchgate.net/publication/256606748_Performance_Comparison_between_RGB_and_HSV_Color_Segmentations_for_Road_Signs_Detection). The image below shows how the RGB colour space can be transformed into the HSV color space, allowing for easier manipulation of colors based on hue, saturation, and value [(Image Reference)](https://www.researchgate.net/publication/383987216_Tongue_color_parameters_in_predicting_the_degree_of_coronary_stenosis_a_retrospective_cohort_study_of_282_patients_with_coronary_angiography).

<p align="center">
  <img width="637" height="303" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/rgb_vs_hsv.png">
</p> 

The figures below were produced using code from [here](https://github.com/ME3-HECM/final-project-ultramen/blob/main/Color_calibration.ipynb) where data was captured using serial communication with the PIC to plot different RGB values for all the colours needed for the maze. This was done using functions in [serial.c](https://github.com/ME3-HECM/final-project-ultramen/blob/main/serial.c) and [interrupts.c](https://github.com/ME3-HECM/final-project-ultramen/blob/main/interrupts.c) for the circular buffer. Realterm was used to receive the serial data from the PIC and store it in a text file using the capture function. During the colour reading, the colour cards were first held very close to the colour sensor and then after a minute of recording data, the card was pulled further away to get a range of values and see how the colour reading change with distance.    
Additionally, the HSV values of the same colours are also plotted on a graph, by converting the RGB readings of each colour using a function called rgb_to_hsv in [Color_calibration python file](https://github.com/ME3-HECM/final-project-ultramen/blob/main/Color_calibration.ipynb). In the HSV plots, it can be seen that colour separation is a lot clearer in comparison to RGB values which makes it a lot easier to negate overlap in the tolerance bands for the colour detection. It is important to note the sporadic nature of the Val plot is due to the fact that Val is a measure of brightness - therefore it varies very easily depending on light conditions. Therefore, using hue and saturation can be used to separate majority of colours and val is used to distinguish between light and dark colours e.g. light blue and blue. 

<p align="center">
  <img width="842" height="827" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/rgb_research.png"> 
  <img width="1018" height="288" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/hsv_research.png">
</p> 

In order to minimize effects of different lighting conditions - a black cover was made to put on the sides of the colour switch board in order to keep lighting conditions consistent no matter the room the buggy is in - this significantly helped improve colour detection performance. The buggy can be seen below.
<p align="center">
  <img width="500" height="400" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/Buggy_image.jpg"> 
</p> 

### 4.2 Calibration
Calibration is necessary to ensure the buggy accurately performs the actions asked of it, and is carried out in [calibrate.c](###-6.5-calibrate.c). Deciding on a unit tile-based movement system (rather than time based) meant that the delay to enact a unit tile distance movement was a mandatory constant to calibrate. In order make accurate turns, a constant that converted an input angle (e.g. 45 degrees) into a delay time for the motors was also necessary. Due to our implementation of turning which involved repeatedly turning 45 degrees to make up larger turns, only one turning constant was required for calibration. This is done by using the setup shown below to make sure a full 180 degree turn is complete when going through the angle calibration steps. Further into the process, we realised that our buggy had a tendency to drift to the right, meaning the left motor was underpowered. We therefore added a calibration function that would vary a small amount of extra power added to the left side motor to improve straight line and turning performance. Finally, we noticed that the half tile movement that was performed to bring the buggy back to the centre of the tile after touching a wall was not exactly half of the full tile time delay, so calibration was needed for this. Using the included buttons, our calibration allows for large adjustments followed by fine-tuning, pressing both buttons at the same time to continue to the next calibration stage. Overall, 4 constants were calibrated: {angle_constant, one_tile, half_tile, motor_imbalance}. These constants were stored in and passed to all functions using a calibration constant structure to keep them together. Initial constant values were found earlier through experimentation and were stored to speed up calibration during the demonstration.

<p align="center">
  <img width="400" height="248" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/Protractor_with_buggy.jpg">
</p> 
<h3 align="center">Buggy Angle Calibration Method</h3>

### 4.3 Buggy Control
For buggy control, movement is controlled using [dc_motor.c](###-6.3-dc_motor.c) to carry out the movements corresponding to the colour detected. This is controlled by the function colour_instructions. This takes in an input of a colour and performs actions within it, along with returning 1 to tell main.c that the function is still ongoing, or returning 0 to tell main.c that operation has ended. Colour_instructions begins by taking the colour passed to it and pushing it to a stack. A stack is particular data structure that operates using Last-In-First-Out principles, and is defined in the file  [stack.c](###-6.4-stack.c) . Pushing a colour to a stack adds it to the top by keeping and updating a pointer to the greatest number element of the colour storage array and adding the colour there. Popping a colour from a stack removes this top element, then decrements the top pointer to point to one element down the stack. This data structure is perfect for a maze finding/returning situation where movements need to be added sequentially then repeated in reverse order one-by-one.

<p align="center">
  <img width="400" height="248" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/stack.webp"> 
</p> 
<h3 align="center">Stack Diagram</h3>

The buggy movement is as follows: move forward one tile until a colour is detected, then push into the wall for alignment, reverse half a tile to centre the buggy, then execute the colour action. This is performed in the function using a central switch statement that has a separate case for each colour input, represented by unsigned chars from 0-8. Each switch case ends with a break statement to ensure the next case isn't executed. The extra actions following a colour input were programmed at the start, one layer outside the switch statement, to ensure they activated every time colour_instructions takes in a colour that is not 0. Delays are found throughout the function to ensure the buggy properly settles before performing the next move, ensuring the accuracy of subsequent colour_instruction calls. Below is a diagram representing the buggy movement path for a relatively simple maze, ending with the green vector.

<p align="center">
  <img width="400" height="248" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/Buggy_path_2.jpeg"> 
</p> 
<h3 align="center">Simple Maze Diagram and Path</h3>

### 4.4 White Instruction
When colour_instructions reads an 8 (representing white), the buggy is set to pause and then execute white_instruction. This is the function responsible for bringing the buggy back to the start of the maze successfully. The function is based around a while loop that continously pops a colour from the top of the stack, and then performs actions according to the popped colour using a switch statement with reversed instructions, with 8 being ignored as the white_instruction is already being carried out. There are two modes of white_instruction: FAST_MODE and SLOW_MODE (represented by !FAST_MODE). In both modes, 0 represents a forward movement of one tile, so this translates to a backwards movement of one tile in white_instruction. Only the first 0 of the stack is skipped using the colour_zero_count in SLOW_MODE, removing the overshoot that would otherwise occur. The buggy in SLOW_MODE retraces the steps of the forward path exactly, pushing into the wall to align itself with every non-0 value popped from the stack. This results in a slow but accurate and self_aligning return to home, and was used in the majority of our maze attempts.

White_instruction also contains a FAST_MODE with slightly different logic. Now, every time a non-0 colour is popped, the colour_zero_count is reset to 0. This leads to all 0 values are followed by another 0 in the stack to be skipped. Along with this, the wall pushes and half tile backsteps are skipped, resulting in the more efficient pathing shown below. Unfortunately, this method does not self-calibrate and therefore small errors accumulated from the buggy not exactly moving a tile or not exactly turning x degrees result in eventual wall collision. However, with a theoretical perfect buggy this return algorithm would result in significant time savings.

<p align="center">
  <img width="400" height="248" src="https://github.com/ME3-HECM/final-project-ultramen/blob/main/gifs/Buggy_path_1.jpeg">
</p> 
<h3 align="center">FAST_MODE Return Path</h3>

When the stack is empty, stack_pop returns 10, indicating that it is empty. This results in an alternate set of actions: the buggy performs a "celebration" move, spinning around and flashing all LEDs, resets colour_zero_count, resets the stack, and returns 0. This return is picked up by colour_instructions, which then returns 0 to main.c. This 0 is finally picked up by an if (!colour_instructions) statement that breaks out of the colour reading/colour execution while loop, and waits back in the previous loop for a button to start a new run. This allows calibration to be saved between runs, which was helpful for the demonstration as our calibration was a long process. Saving it between attempts allowed for an extra run in the given time frame with higher motor max powers and ramp up rates, leading to a time of 1:46 for the fast maze with no touches, as shown in section 5.
### 4.5 Git Usage

Git was used extensively, with both of us working on separate functions in separate branches. These were then reviewed and merged together to keep the main branch clean and containing the most functional code. 197 commits in 8 feature branches were made in total.
## 5. Demonstration videos

### Simple maze completion: [Video](https://www.youtube.com/shorts/sMWcg_Hr5lM)

### Complex maze completion, our fastest time: [Video](https://www.youtube.com/shorts/mEVi8ZEjE7g)

### FAST_MODE - no self-alignment: [Video](https://www.youtube.com/shorts/_tjbY0FXW8E)

## 6. Program Framework
### Function descriptions with returns and arguments can be founded in the .h files.
### 6.1 main.c
- execute necessary inits
- setup motor, rgb, calibration structs
- run calibration
- main while loop
  	- wait for button press to set FAST_MODE or SLOW_MODE
  	- read colour and execute colour instruction
  	- reset to start of loop if 0 returned

### 6.2 color.c
- Execute initiations and turn on Red, Green and Blue lights for colour sensing.
- Colours are read using logic from the i2c protocol
- RGB values are converted into HSV values used for colour detection
- A function determines what colour is read using a threshold of HSV values - otherwise ambient light is read and that colour is returned to [main.c](###-6.1-main.c)

### 6.3 dc_motor.c
- init and dc_motor PWM functions from lab 6 to apply values to motors
- change_motor_speed function generalises process of changing motor power between values within a given increment
- stop function changes speed to 0
- turn_left and turn_right functions execute 45 degree turns (angle / 45) times with a delay in between to let the buggy settle
- full_speed, full_speed_half and max_speed all move the buggy in a straight line for different purposes
	- one tile movement, half tile movement and wall push alignment respectively
- white_instruction performs the return to home algorithm, popping from the stack and executing each colour inverse until empty
- colour_instruction is called in main, and takes a colour, pushes it to a stack, then executes movements depending on the colour

### 6.4 stack.c
- Stack_init sets up a stack with a predefined max number of items
- stack_push adds an item to the top of the stack unless the item would exceed the storage size of the stack (in which case the buggy sees white and returns to the start)
- stack_pop takes off the item on the top of the stack and returns it, unless the stack is empty (in which case it returns 10 to activate buggy celebration)

### 6.5 calibrate.c
- calibration init to setup LEDs for clearer demonstration of calibration steps
- calibrate_angle used to set 45 degree turns looking at 4 in a row (180 degrees) and pressing the left button if the rotation is too far, and right button if rotation is not far enough
	- double button press moves to fine tuning of angle
 - calibrate_distance used to set 1 tile distance by moving 1 tile and pressing the left button if too far, and right button if too short
 	- double button press moves to fine tuning of tile
- calibrate_half used to set half tile distance by moving half a tile and pressing the left button if too far, and right button if too short
	- double button press moves to fine tuning of half tile
 - calibrate_imbalance used to increase/decrease constant added to left motor power with left and right buttons respectively