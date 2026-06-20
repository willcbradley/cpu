#include <stdio.h>
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

  for (int i=0; i < 3; i++) {
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

int main() {
  // initialise latch values to arbitrary values; in hardware, this is randomised
  Latch flipflop = {0, 1};
  Latch master = {0, 1};
  Latch slave = {1, 0};

  dflipflop_update(&flipflop, &master, &slave, 1); // clock 0 -> 1
  print_state("Input D = 1", &flipflop);

  dflipflop_update(&flipflop, &master, &slave, 0); // clock 1 -> 0
  print_state("Input D = 0", &flipflop);

  return 0;
}
