#ifndef REGISTER_GROUP_H
#define REGISTER_GROUP_H
#include <cstdint>

class RegisterGroup {
public:
  char registerA;
  char registerB;
  char registerC;
  char registerX;
  char registerY;
  uint16_t programCounter;
  uint16_t stackPointer;
  char PCB[4];  //this may need to be longer
  unsigned PCBPos;  //where next fetched instruction goes
  void clearPCB();
  RegisterGroup();
};

#endif
