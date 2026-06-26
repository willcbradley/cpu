#include <stdio.h>
#include <stdint.h>
#include "gates.h"
#include "alu.h"
#include "registers.h"
#include "instructions.h"
#include "cpu.h"

void main() {
  // declare CPU; initialise halt variable as off
  CPU cpu;
  cpu.halt = 0;

  // initialise program counter
  register32_init(&(cpu.PC));
  // initialise all registers
  for (int i = 0; i < 8; i++) { 
    register32_init(&(cpu.reg[i]));
  }

  // load instructions onto ROM - since no RAM, manually input values into rs1 for LOAD opcode
  cpu.ROM[0] = encode((uint8_t)LOAD, (uint8_t)0, (uint8_t)5, (uint8_t)0); // reg[0] = 5
  cpu.ROM[1] = encode((uint8_t)LOAD, (uint8_t)1, (uint8_t)7, (uint8_t)0); // reg[1] = 7
  cpu.ROM[2] = encode((uint8_t)ADD, (uint8_t)2, (uint8_t)0, (uint8_t)1); // reg[2] = reg[0] + reg[1]
  cpu.ROM[3] = encode((uint8_t)HALT, (uint8_t)0, (uint8_t)0, (uint8_t)0); // stop

  // initial checks
  printf("\nInitial checks:\n");
  printf("reg[0]: %d\nreg[1]: %d\nreg[2]: %d\nreg[3]: %d\nreg[4]: %d\nreg[5]: %d\nreg[6]: %d\nreg[7]: %d\n\n", register32_read(&(cpu.reg[0])), register32_read(&(cpu.reg[1])), register32_read(&(cpu.reg[2])), register32_read(&(cpu.reg[3])), register32_read(&(cpu.reg[4])), register32_read(&(cpu.reg[5])), register32_read(&(cpu.reg[6])), register32_read(&(cpu.reg[7])));
  printf("Halt: %d\n\n", cpu.halt);
  printf("====================================================================\n\n");

  // run instructions until halted
  while (NOT(cpu.halt)) {
    uint32_t instr = cpu.ROM[register32_read(&(cpu.PC))];
    printf("Encoded instruction is: %u\n", instr);
    Decoded decoded = decode(instr);
    printf("Decoded opcode is: %u\nDecoded rd is: %u\nDecoded rs1 is: %u\nDecoded rs2 is: %u\n", decoded.opcode, decoded.rd, decoded.rs1, decoded.rs2);
    printf("PC: %d\n", register32_read(&(cpu.PC)));
    register32_update(&(cpu.PC), (uint32_t)ripple_adder(register32_read(&(cpu.PC)), 1)); // iterate to next instruction (unless halted)
    execute(&cpu, decoded);
    printf("reg[0]: %d\nreg[1]: %d\nreg[2]: %d\nreg[3]: %d\nreg[4]: %d\nreg[5]: %d\nreg[6]: %d\nreg[7]: %d\n", register32_read(&(cpu.reg[0])), register32_read(&(cpu.reg[1])), register32_read(&(cpu.reg[2])), register32_read(&(cpu.reg[3])), register32_read(&(cpu.reg[4])), register32_read(&(cpu.reg[5])), register32_read(&(cpu.reg[6])), register32_read(&(cpu.reg[7])));
    printf("Halt: %d\n\n", cpu.halt);
  }

  // Print value of R2 - if this equals 12, we know everything is working
  printf("With default values, if everything is working, the following number will be twelve:\n%d\n", (int)register32_read(&(cpu.reg[2])));
}
