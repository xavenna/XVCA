#ifndef FLAGS_H
#define FLAGS_H



// Bit pattern:
// n/a, n/a, n/a, greater, equal, zero, sign, carry

class FlagRegister {
public:
  bool carry;
  bool zero;
  bool equal;
  bool greater;
  bool sign;
  char reg();
  void assignReg(char);
  FlagRegister();
};

#endif
