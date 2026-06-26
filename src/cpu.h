#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
  Register32 PC;
  Register32 reg[8]; // 8-bit register - turns out register is a reserved word !!!
  uint32_t ROM[256]; // arbitrary size
  int halt;
} CPU;

void execute(CPU *cpu, Decoded d);

#endif
