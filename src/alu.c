#include <stdio.h>
#include <stdint.h>
#include "gates.h"

void half_adder(unsigned int a, unsigned int b, unsigned int *sum, unsigned int *carry) {
    *sum = XOR(a,b);
    *carry = AND(a,b);
}

void half_adder_table() {
    printf("Half adder truth table:\n\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            unsigned int s, c;
            half_adder(i, j, &s, &c);
            printf("A: %d\nB: %d\nSum: %d\nCarry: %d\n\n", i, j, s, c);
        }
    }
    printf("\n");
}

void full_adder(unsigned int a, unsigned int b, unsigned int carry_in, unsigned int *sum, unsigned int *carry_out) {
    *sum = XOR((XOR(a, b)), carry_in);
    *carry_out = OR(AND(XOR(a, b), carry_in),AND(a,b));
}

void full_adder_table() {
    printf("Full adder truth table:\n\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                unsigned int s, c;
                full_adder(i, j, k, &s, &c);
                printf("A: %d\nB: %d\nCarry In: %d\nSum: %d\nCarry Out: %d\n\n", i, j, k, s, c);
            }
        }
    }
    printf("\n");
}

unsigned int bit_count(unsigned int num) {
    if (num == 0) return 1;
    unsigned int count = 0;
    while (num > 0) {
        num = num >> 1;
        count++;
    }
    return count;
}

unsigned int max_bit_count(unsigned int a, unsigned int b) {
    if (bit_count(a) > bit_count(b)) return bit_count(a);
    else return bit_count(b);   
}

unsigned int ripple_adder(unsigned int a, unsigned int b) {
    unsigned int carry = 0, A = 0, B = 0, result = 0;
    unsigned int s, c;
    for (int i = 0; i <= (max_bit_count(a, b)); i++) {
        // access the i'th bit of both numbers by right shifting
        // use built-in bitwise logic operators for multi-bit comparisons
        A = (a >> i) & 1;
        B = (b >> i) & 1;
        // apply full adder to current bits
        full_adder(A, B, carry, &s, &c);
        // left shifting by i aligns sum w/ correct bit; using OR turns bit on if sum from full adder is 1
        result = result | (s << i);
        // carry reassigned for next loop
        carry = c;
    }
    return result; 
}
