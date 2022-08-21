#include <iostream>
#include "emulator.h"


bool Emulator::runCycle() {
  //fetch instruction from memory at program counter into PCB
  cpu.fetchInsToPCB();
  //for some reason, it always fetches a NOP
  //now it segfaults instead
  
  //check if PCB contains a complete command
  if(cpu.pcbIsValidIns()) {
    //if so, execute it
    cpu.executeInstruction();
    cpu.registers.clearPCB();
  }
  else {
    //otherwise, end cycle
    return true;  //I'm not sure if this should be here
  }


  //update hardware
  return updateHardware();
}

void Emulator::setDrive(std::string name) {
  driveName = name;
  adapterGroup.driveAdapter.driveName = name;
}

bool Emulator::updateHardware() {
  //check for drive calls
  
  adapterGroup.driveAdapter.update();

  //check for shutdown
  if((uint8_t)adapterGroup.shutdownBuf.buffer == 0xff) {
    //shutdown
    return false;
  }
  //update display
  //not now, for debugging reasons
  //adapterGroup.displayAdapter.updateDisplay();
  //update keyboard
  bool interrupt = adapterGroup.keyboardAdapter.updateBuffer();
  return interrupt;  //false if shutdown
}

Emulator::Emulator() : cpu{adapterGroup} {
}

Emulator::Emulator(AdapterGroup& ag) : adapterGroup{ag}, cpu{ag}  {
  
}
