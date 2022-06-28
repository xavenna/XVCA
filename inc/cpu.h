#ifndef CPU_H
#define CPU_H
#include "memory-group.h"
#include "register-group.h"
#include "adapter-group.h"

class CPU {
public:
  MemoryGroup memory;
  RegisterGroup registers;
  void fetchInsToPCB();
  bool pcbIsValidIns();
  void executeInstruction();
  CPU(AdapterGroup);
};


#endif
