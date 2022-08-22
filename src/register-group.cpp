#include "register-group.h"

void RegisterGroup::clearPCB() {
  for(int i=0;i<4;i++) {
    PCB[i] = 0;
  }
  PCBPos = 0;
}

uint16_t RegisterGroup::XY() {
  return (registerX << 8) | registerY;
}
void RegisterGroup::decSP() {
  if(stackPointer == 0) {
    stackPointer = 65535;
  }
  else {
    stackPointer--;
  }
}
void RegisterGroup::incSP() {
  if(stackPointer == 65535) {
    stackPointer = 0;
  }
  else {
    stackPointer++;
  }
}
RegisterGroup::RegisterGroup() {
  registerA = 0;
  registerB = 0;
  registerC = 0;
  registerX = 0;
  registerY = 0;
  programCounter = 0;
  stackPointer = 0;
  clearPCB();
}
