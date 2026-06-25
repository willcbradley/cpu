#include <stdio.h>
#include <stdint.h>
#include "gates.h"

uint32_t encode(uint8_t opcode, uint8_t rd, uint8_t rs1, uint8_t rs2) {
  // Or mask maps 1 to any blank areas if it exists in the original instruction
  return OR(OR(OR((opcode << 24), (rd << 16)), (rs1 << 8)), rs2);
}

typedef struct {
    uint8_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
} DecodedInstruction;

DecodedInstruction decode(uint32_t instr) {
    DecodedInstruction d;
    // 0xFF is 255, meaning one off 2^8, meaning 8 ones (since first digit is 2^0); hence masking isolates correct part of instructoin
    d.opcode = (instr >> 24) & 0xFF;
    d.rd = (instr >> 16) & 0xFF;
    d.rs1 = (instr >> 8) & 0xFF;
    d.rs2 = instr & 0xFF;
    return d;
}
