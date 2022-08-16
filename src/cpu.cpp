#include "cpu.h"
#include "emulator.h"



void CPU::fetchInsToPCB() {
  char temp = memory.read(registers.programCounter);
  registers.PCB[registers.PCBPos] = temp;
  registers.PCBPos++;
}

void CPU::executeInstruction() {
  uint16_t address;
  switch(registers.PCB[0]) {
    //this needs to be redone for XVCA=-p2
  case 0x0:
  default:
    //NOP
    //do nothing
    break;
  case 0x1:
    //MVRA <r>
    //move specified register to register A
    switch (registers.PCB[1]) {
    case 0:
      //does nothing
      break;
    case 1:
      registers.registerA = registers.registerB;
      break;
    case 2:
      registers.registerA = registers.registerC;
      break;
    case 3:
      registers.registerA = registers.registerX;
      break;
    case 4:
      registers.registerA = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    break;
  case 0x2:
    //MVRB <r>
    switch (registers.PCB[1]) {
    case 0:
      registers.registerB = registers.registerA;
      break;
    case 1:
      //does nothing
      break;
    case 2:
      registers.registerB = registers.registerC;
      break;
    case 3:
      registers.registerB = registers.registerX;
      break;
    case 4:
      registers.registerB = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    break;
  case 0x3:
    //MVRC <r>
    switch (registers.PCB[1]) {
    case 0:
      registers.registerC = registers.registerA;
      break;
    case 1:
      registers.registerC = registers.registerB;
      break;
    case 2:
      //does nothing
      break;
    case 3:
      registers.registerC = registers.registerX;
      break;
    case 4:
      registers.registerC = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    break;
  case 0x4:
    //MVRX <r>
    switch (registers.PCB[1]) {
    case 0:
      registers.registerX = registers.registerA;
      break;
    case 1:
      registers.registerX = registers.registerB;
      break;
    case 2:
      registers.registerX = registers.registerC;
      break;
    case 3:
      //does nothing
      break;
    case 4:
      registers.registerX = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    break;
  case 0x5:
    //MVRY <r>
    switch (registers.PCB[1]) {
    case 0:
      registers.registerY = registers.registerA;
      break;
    case 1:
      registers.registerY = registers.registerB;
      break;
    case 2:
      registers.registerY = registers.registerC;
      break;
    case 3:
      registers.registerY = registers.registerX;
      break;
    case 4:
      //does nothing
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    break;
  case 0x6:
    //MVVA <v>
    registers.registerA = registers.PCB[1];
    break;
  case 0x7:
    //MVVB <v>
    registers.registerB = registers.PCB[1];
    break;
  case 0x8:
    //MVVC <v>
    registers.registerC = registers.PCB[1];
    break;
  case 0x9:
    //MVVX <v>
    registers.registerX = registers.PCB[1];
    break;
  case 0xa:
    //MVVY <a>
    registers.registerY = registers.PCB[1];
    break;
  case 0xb:
    //MVMA <a>
    registers.registerA = memory.read((registers.PCB[1] << 8) & registers.PCB[2]);
    break;
  case 0xc:
    //MVMB <a>
    registers.registerB = memory.read((registers.PCB[1] << 8) & registers.PCB[2]);
    break;
  case 0xd:
    //MVMC <a>
    registers.registerC = memory.read((registers.PCB[1] << 8) & registers.PCB[2]);
    break;
  case 0xe:
    //MVMX <a>
    registers.registerX = memory.read((registers.PCB[1] << 8) & registers.PCB[2]);
    break;
  case 0xf:
    //MVMY <a>
    registers.registerY = memory.read((registers.PCB[1] << 8) & registers.PCB[2]);
    break;
  case 0x10:
    //MVIA
    registers.registerA = memory.read((registers.registerX << 8) & registers.registerY);
    break;
  case 0x11:
    //MVIB
    registers.registerB = memory.read((registers.registerX << 8) & registers.registerY);
    break;
  case 0x12:
    //MVIC
    registers.registerC = memory.read((registers.registerX << 8) & registers.registerY);
    break;
  case 0x13:
    //MVIX
    registers.registerX = memory.read((registers.registerX << 8) & registers.registerY);
    break;
  case 0x14:
    //MVIY
    registers.registerY = memory.read((registers.registerX << 8) & registers.registerY);
    break;
  }
}


bool CPU::pcbIsValidIns() {
  if(registers.PCBPos == 0) {
    return false;
  }
  else if(registers.PCBPos == 1) {
    switch(registers.PCB[0]) {
      
    }
    
  }
  else if(registers.PCBPos == 2) {
    
  }
  else if(registers.PCBPos == 3) {
    
  }
  return false;
}

CPU::CPU(AdapterGroup ad) : memory{ad.driveAdapter.driveBuf, ad.displayAdapter.displayBuf, ad.keyboardAdapter.keyboardBuffer, ad.driveAdapter.drcBuf, ad.shutdownBuf} {
  
}
