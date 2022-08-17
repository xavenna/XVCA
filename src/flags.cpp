#include "flags.h"


// n/a, n/a, n/a, greater, equal, zero, sign, carry

char FlagRegister::reg() {
  return (greater << 4) & (equal << 3) & (zero << 2) & (sign << 1) & carry;
}
void FlagRegister::assignReg(char flags) {
  carry = flags & 1;
  sign = flags & 2;
  zero = flags & 4;
  equal = flags & 8;
  greater = flags & 16;
}
FlagRegister::FlagRegister() {
  carry = false;
  zero = false;
  sign = false;
  equal = false;
  greater = false;
}
