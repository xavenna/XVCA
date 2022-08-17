#ifndef REGISTER_GROUP_H
#define REGISTER_GROUP_H
#include <cstdint>

#include "flags.h"

class RegisterGroup {
public:
  char registerA;
  char registerB;
  char registerC;
  char registerX;
  char registerY;
  FlagRegister flags;
  uint16_t programCounter;
  uint16_t stackPointer;
  uint8_t PCB[4];  //this may need to be longer
  unsigned PCBPos;  //where next fetched instruction goes
  void clearPCB();
  uint16_t XY();
  RegisterGroup();
};

#endif
