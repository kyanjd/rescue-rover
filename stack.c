#include <xc.h>
#include "stack.h"


void Stack_init(Stack *s)
{
    s->top = -1; //initialise stack at -1 so first item goes in to position 0
}

void stack_push(Stack *s, unsigned char colour) 
{
    if (s->top < 99) { // 99 colours is the max the stack can store
        s->top++; // increases the top stack index position 
        s->colour[s->top] = colour; // assigns colour to the top of the stack        
    }
    else {
        s->colour[s->top] = 8; // white colour put the the top of the stack so that the buggy goes back to its start position
    }
}

unsigned char stack_pop(Stack *s) 
{
    if (s->top >= 0) {
        unsigned char colour = s->colour[s->top];
        s->top--;
        return colour;
    }
    return 10;  // return 10 to signify empty stack
}


