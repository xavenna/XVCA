#ifndef REGISTER_GROUP_H
#define REGISTER_GROUP_H
#include <cstdint>
#include <array>

#include "flags.h"

class RegisterGroup {
public:
  uint8_t registerA;
  uint8_t registerB;
  uint8_t registerC;
  uint8_t registerX;
  uint8_t registerY;
  FlagRegister flags;
  uint16_t programCounter;
  uint16_t stackPointer;
  std::array<uint8_t, 4> PCB;  //this may need to be longer
  unsigned PCBPos;  //where next fetched instruction goes
  void clearPCB();
  uint16_t XY();
  void decSP();
  void incSP();
  RegisterGroup();
};

#endif
