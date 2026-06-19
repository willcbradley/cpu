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

int main() {
  // initialise values to arbitrary values; in hardware, this is randomised
  Latch latch = {0, 1};

  print_state("Initial state:", &latch);

  dlatch_update(&latch, 0, 0);
  print_state("D=0, E=0", &latch);

  dlatch_update(&latch, 1, 0);
  print_state("D=1, E=0", &latch);

  dlatch_update(&latch, 0, 1);
  print_state("D=0, E=1", &latch);

  dlatch_update(&latch, 1, 1);
  print_state("D=1, E=1", &latch);

  return 0;
}

// todo: make d flip flop using the above
