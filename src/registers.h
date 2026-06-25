#include <stdio.h>
#include <stdint.h>
#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct {
  int Q;
  int Qn;
} Latch;

void srlatch_update(Latch *latch, int S, int R);

void dlatch_update(Latch *latch, int D, int E);

void print_state(const char *label, Latch *latch);

void dflipflop_update_falling(Latch *flipflop, Latch *master, unsigned int D, unsigned int clock);

void dflipflop_update_rising(Latch *flipflop, Latch *master, Latch *slave, unsigned int clock);

#define WIDTH 32

typedef struct {
  Latch master[WIDTH];
  Latch slave[WIDTH];
  Latch flipflop[WIDTH];
} Register32;

void register32_init(Register32 *reg);

void register32_update_falling(Register32 *reg, uint32_t D, unsigned int clock);

void register32_update_rising(Register32 *reg, unsigned int clock);

void register32_update(Register32 *reg, uint32_t D);

uint32_t register32_read(Register32 *reg);

#endif
