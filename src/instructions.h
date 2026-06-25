#include <stdio.h>
#include <stdint.h>
#ifndef DECODER_H
#define DECODER_H

typedef struct {
    uint8_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
} DecodedInstruction;

DecodedInstruction decode(uint32_t instr);

#endif
