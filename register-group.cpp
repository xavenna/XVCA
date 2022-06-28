#include "register-group.h"

void RegisterGroup::clearPCB() {
  for(int i=0;i<4;i++) {
    PCB[i] = 0;
  }
}

RegisterGroup::RegisterGroup() {
  registerA = 0;
  registerX = 0;
  registerY = 0;
  programCounter = 0;
  stackPointer = 0;
}
