#include <stdio.h>
#include <stdint.h>
#include "pastlogic.h"

// create NOR gate from previous logic
unsigned int NOR(unsigned int param1, unsigned int param2) {
    return NOT(OR(param1, param2));
}

/* ---------- SR Latch ----------- */
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

/* ---------- D Latch ----------- */
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
// define clock global variable first so that clock_tick function definition compiles correctly
// clock idles at one to enable falling edge first (update master) then rising ege (update slave)
unsigned int clock = 1;

void clock_tick(void) {
  clock = NOT(clock);
}

void dflipflop_update(Latch *flipflop, Latch *master, Latch *slave, unsigned int D) { // flipflop has same members -> same typedef
  // define function scope variables
  unsigned int q = flipflop->Q;
  unsigned int qn = flipflop->Qn;

  clock_tick(); // falling edge
  dlatch_update(master, D, NOT(clock)); // compute master latch output
  
  clock_tick(); // rising edge
  dlatch_update(slave, master->Q, clock); // compute slave latch output

  // compute new flipflop outputs
  unsigned int newQ = slave->Q;
  unsigned int newQn = slave->Qn;

  // redefine function-scope variables
  q = newQ;
  qn = newQn;

  // update flipflop outputs at struct level
  flipflop->Q = q;
  flipflop-> Qn = qn;
}

/* ---------- D Flipflop ----------- */
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

void register32_update(Register32 *reg, uint32_t D) {
  clock_tick(); // falling edge -> master latches updates
  for (int i = 0; i < 32; i++) {
    unsigned int bit = (D >> i) & 1u; // right shift by i to access ith bit, compare to 00000001 to cancel all else, and determine 1/0 of bit of interest
    dlatch_update(&reg->master[i], bit, NOT(clock));
  }

  clock_tick(); // rising edge -> update slave latches
  for (int i = 0; i < 32; i++) {
    dlatch_update(&reg->slave[i], reg->master[i].Q, clock);
    // output of slave latch becomes output of flipflop
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

int main() {
  Register32 reg;
  register32_init(&reg);

  printf("Initial:               0x%08X\n", register32_read(&reg)); // 0 means pad w/ zeros on left, 8 means 8-wide, X means hex
  
  register32_update(&reg, 0xDEADBEEF);
  printf("After load 0xDEADBEEF: 0x%08X\n", register32_read(&reg));

  register32_update(&reg, 0x00000000);
  printf("After load 0x00000000: 0x%08X\n", register32_read(&reg));

  register32_update(&reg, 0xCAFEF00D);
  printf("After load 0xCAFEF00D: 0x%08X\n", register32_read(&reg));

  return 0;
}
