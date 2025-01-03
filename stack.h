#ifndef _stack_H
#define _stack_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

/********************************************//**
 *  Defining the Stack struct to store colours efficiently using Last In First Out logic
 ***********************************************/
typedef struct Stack { // 
    int top;
    unsigned char colour[100];
} Stack;

/********************************************//**
 *  Function to initiate stack 
 ***********************************************/
void Stack_init(Stack *s);

/********************************************//**
 *  Function to push colours onto the stack if there's space
 ***********************************************/
void stack_push(Stack *s, unsigned char colour);

/********************************************//**
 *  Function to return the top colour from the stack if it exists
 ***********************************************/
unsigned char stack_pop(Stack *s);

#endif
