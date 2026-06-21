#ifndef ALU_H
#define ALU_H
#include <stdio.h>
#include <stdint.h>

void half_adder(unsigned int a, unsigned int b, unsigned int *sum, unsigned int *carry);

void half_adder_table();

void full_adder(unsigned int a, unsigned int b, unsigned int carry_in, unsigned int *sum, unsigned int *carry_out);

void full_adder_table();

unsigned int bit_count(unsigned int num);

unsigned int max_bit_count(unsigned int a, unsigned int b);

unsigned int ripple_adder(unsigned int a, unsigned int b);

#endif
