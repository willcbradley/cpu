#include <stdio.h>
#include <stdint.h>
#include "../src/alu.h"

int main() {
  half_adder_table();
  full_adder_table();
  int num1, num2;
  printf("Type two numbers to be added (separated by a space): ");
  scanf("%d %d", &num1, &num2);
  printf("%d", ripple_adder(num1, num2));
  return 0;
}
