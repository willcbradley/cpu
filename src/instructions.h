#include <stdio.h>
#include <stdint.h>
#ifndef DECODER_H
#define DECODER_H

enum opcode {
  LOAD = 0, ADD = 1, JMP = 2, HALT = 3
};

uint32_t encode(uint8_t opcode, uint8_t rd, uint8_t rs1, uint8_t rs2);

typedef struct {
    uint8_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
} Decoded;

Decoded decode(uint32_t instr);

#endif
