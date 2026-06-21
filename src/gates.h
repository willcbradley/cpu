#ifndef GATES_H
#define GATES_H
#include <stdio.h>
#include <stdint.h>

unsigned int AND(int param1, int param2);

unsigned int OR(int param1, int param2);

unsigned int NOT(int value);

unsigned int XOR(int param1, int param2);

unsigned int NOR(unsigned int param1, unsigned int param2);

void ANDTABLE();

void ORTABLE();

void NOTTABLE();

void XORTABLE();

void NORTABLE();

#endif
