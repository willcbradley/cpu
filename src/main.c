#include <stdio.h>
#include <stdint.h>
#include "gates.h"
#include "alu.h"
#include "registers.h"
#include "instructions.h"

uint32_t ROM[3]; // hold three instructions: load, add, halt
Register32 PC; // define variable so have something to pass an address to for the init function below
register32_init(&PC); //intiialise all Q values to zero (hence PC starts at zero)

void main() {

}
