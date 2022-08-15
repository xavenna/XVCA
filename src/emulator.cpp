#include <iostream>
#include "emulator.h"


bool Emulator::runCycle() {
  //fetch instruction from memory at program counter into PCB
  cpu.fetchInsToPCB();
  //check if PCB contains a complete command
  if(cpu.pcbIsValidIns()) {
    //if so, execute it
    cpu.executeInstruction();
  }
  else {
    //otherwise, end cycle
  }


  //update hardware
  return updateHardware();
}

void Emulator::setDrive(std::string name) {
  driveName = name;
}

bool Emulator::updateHardware() {
  //check for drive calls
  //check for shutdown
  //update display
  adapterGroup.displayAdapter.updateDisplay();
  return true;  //false if shutdown
}

Emulator::Emulator() : cpu{adapterGroup} {
  
}
