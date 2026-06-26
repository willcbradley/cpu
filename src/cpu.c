#include <stdio.h>
#include <stdint.h>
#include "gates.h"
#include "alu.h"
#include "registers.h"
#include "instructions.h"

typedef struct {
  Register32 PC;
  Register32 reg[8]; // 8-bit register - turns out register is a reserved word !!!
  uint32_t ROM[256]; // arbitrary size
  int halt;
} CPU;

void execute(CPU *cpu, Decoded d) {
    switch (d.opcode) {
          case LOAD:
              register32_update(&(cpu->reg[d.rd]), (uint32_t)d.rs1);
              break;

          case ADD:
              register32_update(&(cpu->reg[d.rd]), (uint32_t)ripple_adder(register32_read(&(cpu->reg[d.rs1])), register32_read(&(cpu->reg[d.rs2]))));
              break;

          case JMP:
              register32_update(&(cpu->PC), d.rd); // jump to new instruction point as specified
              break;

          case HALT:
              cpu->halt = 1;
              break;
    }
}
