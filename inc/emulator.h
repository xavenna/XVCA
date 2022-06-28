#ifndef EMULATOR_H
#define EMULATOR_H
#include <cstdint>
#include <string>
//maybe split classes into different headers
#include "register-group.h"
#include "memory-group.h"
#include "adapter-group.h"
#include "cpu.h"




class Emulator {
public:
  CPU cpu;
  AdapterGroup adapterGroup;
  std::string driveName;
  
  void setDrive(std::string);
  bool updateHardware();
  bool runCycle();
  Emulator();
};
#endif
