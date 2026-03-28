#include <stdio.h>
#include "pastlogic.h"

// create NOR gate from previous logic
unsigned int NOR(unsigned int param1, unsigned int param2) {
    return NOT(OR(param1, param2));
}

// use NOR gate to build SR latch  
void gated_latch(unsigned int D, unsigned int enable, unsigned int *q) {
    // calculate set and reset
    unsigned int set = AND(D, enable);
    unsigned int reset = AND(NOT(D), enable);

    // update memory state (or not) accordingly
    unsigned int q_next = NOR(reset, NOR(set, *q));

    *q = q_next;
}


// next question to be solved: does q need initialising somehow when the latch is "powered on"?