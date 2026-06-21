#include <stdio.h>
#include <stdint.h>
#include "../src/registers.h"

int main() {
  int clock = 1; // clock MUST idle at 1

  // create flipflop
  Latch flipflop = {0, 1};
  Latch master = {0, 1};
  Latch slave = {0, 1};

  // create and initialise 32 bit register
  Register32 reg;
  register32_init(&reg);

  printf("Flip-Flop, master/slave latches, and Register32 initialised\n\n");
  printf("Flip-Flop -> Q: %d, Qn: %d\n", flipflop.Q, flipflop.Qn);
  printf("Register 32:               0x%08X\n\n", register32_read(&reg));

  clock = 0; // simulate clock_tick() falling edge
  printf("clock 1 -> 0 (falling edge)\n");

  dflipflop_update_falling(&flipflop, &master, 1, clock);
  register32_update_falling(&reg, 0xDEADBEEF, clock);
  printf("Master latches updated. Flip-flop/register outputs should remain the same.\n\n");
  printf("Flip-Flop -> Q: %d, Qn: %d\n", flipflop.Q, flipflop.Qn);
  printf("Register 32:               0x%08X\n\n", register32_read(&reg)); // 0 means pad w/ zeros on left, 8 means 8-wide, X means hex

  clock = 1; // simulate clock_tick() rising edge
  printf("clock 0 -> 1 (rising edge)\n");

  dflipflop_update_rising(&flipflop, &master, &slave, clock);
  register32_update_rising(&reg, clock);
  printf("Slave latches/flip-flops updated. Flip-flop should now have Q: 1 Qn: 0, and register should now read 0xDEADBEEF.\n\n");
  printf("Flip-Flop -> Q: %d, Qn: %d\n", flipflop.Q, flipflop.Qn);
  printf("Register 32:               0x%08X\n", register32_read(&reg));

  return 0;
}
