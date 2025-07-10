#include <iostream>
#include "emulator.h"
#include <chrono>
#include <fstream>
#include <iomanip>


// 1 - Shutdown
// 2 - Halt
// 3 - Keyboard Interrupt
int Emulator::runCycle() {
  //fetch instruction from memory at program counter into PCB
  if(!cpu.halted) {
    if(cpu.fetchInsToPCB() != 0) {
      std::cerr << "Exception caught, dumping memory\n";
      char buffer[0x10000];
      cpu.memory.readBlock(0, buffer, sizeof(buffer));
      std::ofstream dump("dump.bin", std::ios::binary);
      dump.write(buffer, sizeof(buffer));
      return 1;
    }
    
    //check if PCB contains a complete command
    if(cpu.pcbIsValidIns()) {
      //if so, execute it
      switch(cpu.executeInstruction()) {
      case -1:
        //halt, do nothing until an interrupt is detected?
        cpu.registers.clearPCB();
        return 2;
      default:
        cpu.registers.clearPCB();
        break;
      }
    }
    else {
      //otherwise, end cycle
      return 0;  //I'm not sure if this should be here
    }
    cycleNum++;

  }

  //update hardware
  switch(updateHardware()) {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      //halt emulator
      break;
    case 3:
      //keyboard interrupt
      // just jump out of a halted state
      //check if interrupts are enabled

      /*
      if(cpu.interruptsEnabled) {
        // push pc for return
        cpu.registers.programCounter++;
        cpu.registers.stackPointer--;
        cpu.memory.write(cpu.registers.stackPointer, (cpu.registers.programCounter >> 8) & 0xff);
        cpu.registers.stackPointer--;
        cpu.memory.write(cpu.registers.stackPointer,cpu.registers.programCounter & 0xff);
        cpu.registers.programCounter = 0x400;
      }
      */
      cpu.halted = false;
      //the cpu state is not reset here, which is an issue 
      return 0;
    case 4:
      //this triggers the interrupt handler?
      //force interrupt? (maybe a NMI?)
      cpu.registers.programCounter++;
      cpu.registers.stackPointer--;
      cpu.memory.write(cpu.registers.stackPointer, (cpu.registers.programCounter >> 8) & 0xff);
      cpu.registers.stackPointer--;
      cpu.memory.write(cpu.registers.stackPointer,cpu.registers.programCounter & 0xff);
      cpu.registers.programCounter = 0x400;
      cpu.halted = false;
      return 0;
  }
  return 0;
}

void Emulator::setDrive(std::string name) {
  driveName = name;
  adapterGroup.driveAdapter.driveName = name;
}

int Emulator::updateHardware() {
  long currentTime = getTimeDiff();
  long timeSinceLastFrame = currentTime - lastFrameEndTime;


  //update different parts at different times
  adapterGroup.driveAdapter.update();
  if(adapterGroup.shutdownBuf.buffer == static_cast<char>(0xff)) {
    std::cerr << "Shutting down\n";
    //shutdown
    return 1;
  }


  if(timeSinceLastFrame < DispUpdateDuration) {
    return 0;
  }

  long long int cycleTime = getPreciseTimeDiff();

  long long int elapsedCycles = cycleNum - lastHardwareUpdateCycleNum;
  lastHardwareUpdateCycleNum = cycleNum;

  
  double frequency = elapsedCycles * 1000 / cycleTime; //this is measured in MHz?
  

  std::cerr << "Clock: "<<frequency << " kHz;"<<cycleTime<<";"<<elapsedCycles<<
      ";PC:"<<std::hex<<std::setw(4)<<cpu.registers.programCounter<<std::dec << "                                 \n";

  //check for drive calls
  
  //if I want this emulator to get any kind of reasonable performance, this needs to be optimized. (maybe use threads)
  adapterGroup.displayAdapter.updateDisplay();
  int interrupt = 0;
  interrupt = adapterGroup.keyboardAdapter.updateBuffer();

  // this element should only be run every so many seconds
  lastFrameEndTime = getTimeDiff();

  return interrupt;  //false if shutdown
}

Emulator::Emulator(AdapterGroup& ag) : adapterGroup{ag}, cpu{ag}  {
  auto start = std::chrono::system_clock::now().time_since_epoch();
  startingTime = std::chrono::duration_cast<std::chrono::microseconds>(start).count();
  lastFrameEndTime = 0;
}

long int Emulator::getTimeDiff() {
  auto end = std::chrono::system_clock::now().time_since_epoch();
  auto p = std::chrono::duration_cast<std::chrono::microseconds>(end).count();
  return p-startingTime;

}

long long int Emulator::getPreciseTimeDiff() {
  auto end = std::chrono::system_clock::now().time_since_epoch();
  auto p = std::chrono::duration_cast<std::chrono::microseconds>(end).count();
  auto q = p - preciseTime;
  preciseTime = p;
  return q;
}
