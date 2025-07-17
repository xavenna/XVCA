#ifndef CPU_H
#define CPU_H

#include <stdexcept>
#include <fstream>

#include "memory-group.h"
#include "register-group.h"
#include "adapter-group.h"

class CPU {
public:
  MemoryGroup memory;
  RegisterGroup registers;
  int fetchInsToPCB();
  bool pcbIsValidIns();
  int executeInstruction();
  CPU(AdapterGroup&);
  bool interruptsEnabled=true;
  bool halted=false;
  
private:
  char regNum(int);
  std::ofstream debug;
  //std::string instructionName();
};


#endif
