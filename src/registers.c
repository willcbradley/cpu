#include <stdio.h>
#include <stdint.h>
#include "gates.h"

typedef struct {
  int Q;
  int Qn;
} Latch;

void srlatch_update(Latch *latch, int S, int R) { // * used in declaration means the value is a pointer (variable that stores memory address)
  unsigned int q = latch->Q; // Arrow operator acccesses struct member through pointer; since declaration above uses asterix "latch" is pointer
  unsigned int qn = latch->Qn;

  for (int i=0; i < 3; i++) { // physical hardware achieves stready state; here, loop a few times to approximate
    // compute new latch outputs
    unsigned int newQ = NOR(R, qn);
    unsigned int newQn = NOR(S, q);
    // redefine function-scope variables
    q = newQ;
    qn = newQn;
  }

  // update struct members based on "steady state"
  latch->Q = q;
  latch->Qn = qn;
}

void dlatch_update(Latch *latch, int D, int E) { // D = data, E = enable
  unsigned int q = latch->Q;
  unsigned int qn = latch->Qn;

  unsigned int S = AND(D, E);
  unsigned int R = AND(NOT(D), E);

  for (int i = 0; i < 3; i++) {
    // compute new latch outputs
    unsigned int newQ = NOR(R, qn);
    unsigned int newQn = NOR(S, q);
    // redefine function-scope variables
    q = newQ;
    qn = newQn;
  }

  latch->Q = q;
  latch->Qn = qn;
}

// print latch results to track updates
void print_state(const char *label, Latch *latch) { // C has no strings; declare as pointer for efficiency (only first letter is stored as value)
  printf("%s -> Q=%d, Qn=%d\n", label, latch->Q, latch->Qn);
}

/* ---------- D Flipflop ----------- */

// IMPORTANT: all clock-sensitive functions assume main.c clock variable (defined within the main() function, rather than globally) idles at 1
// They rely on a clock tick occuring BEFORE being called

void dflipflop_update_falling(Latch *flipflop, Latch *master, unsigned int D, unsigned int clock) { // fliflop has same members -> same Latch typedef
  dlatch_update(master, D, NOT(clock));
}

void dflipflop_update_rising(Latch *flipflop, Latch *master, Latch *slave, unsigned int clock) {
  dlatch_update(slave, master->Q, clock);

  flipflop->Q = slave->Q;
  flipflop->Qn = slave->Qn;
}

/* ---------- 32-bit Register ----------- */
#define WIDTH 32

typedef struct {
  Latch master[WIDTH];
  Latch slave[WIDTH];
  Latch flipflop[WIDTH];
} Register32;

void register32_init(Register32 *reg) {
  for (int i = 0; i < 32; i++) {
    reg->master[i] = (Latch){0, 1};
    reg->slave[i] = (Latch){0, 1};
    reg->flipflop[i] = (Latch){0, 1};
  }
}

void register32_update_falling(Register32 *reg, uint32_t D, unsigned int clock) { // falling edge -> master node updates
  for (int i = 0; i < 32; i++) {
    unsigned int bit = (D >> i) & 1u;
    dlatch_update (&reg->master[i], bit, NOT(clock));
  }
}

void register32_update_rising(Register32 *reg, unsigned int clock) { // rising edge -> slave node (and therefore overarching register) updates
  for (int i = 0; i < 32; i++) {
    // output (Q) of master becomes input to slave
    dlatch_update(&reg->slave[i], reg->master[i].Q, clock);
    // output of slave becomes new register values
    reg->flipflop[i].Q = reg->slave[i].Q;
    reg->flipflop[i].Qn = reg->slave[i].Qn;
  }
}

uint32_t register32_read(Register32 *reg) {
  uint32_t value = 0;
  for (int i = 0; i < 32; i++) {
    if (reg->flipflop[i].Q) {
      value = value | (1u << i); // 1 or 0 is always 1, so this lets us set the i'th bit to 1 if Q = 1
    }
  }
  return value;
}
