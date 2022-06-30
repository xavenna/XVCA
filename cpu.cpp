#include "cpu.h"
#include "emulator.h"



void CPU::fetchInsToPCB() {
  char temp = memory.read(registers.programCounter);
  registers.PCB[registers.PCBPos] = temp;
  registers.PCBPos++;
}

void CPU::executeInstruction() {
  switch(registers.PCB[0]) {
  case '\x0':
  default:
    //NOP
    break;
  case '\x1':
    //MVRA X
    break;
  case '\x2':
    //MVRA Y
    break;
  case '\x3':
    //MVRX A
    break;
  case '\x4':
    //MVRX Y
    break;
  case '\x5':
    //MVRY A
    break;
  case '\x6':
    //MVRY X
    break;
  case '\x7':
    //MVVA <v>
    break;
  case '\x8':
    //MVVX <v>
    break;
  case '\x9':
    //MVVY <v>
    break;
  case '\xa':
    //MVMA <a>
    break;
  case '\xb':
    //MVMX <a>
    break;
  case '\xc':
    //MVMY <a>
    break;
  case '\xd':
    //MVIA
    break;
  case '\xe':
    //MVIX
    break;
  case '\xf':
    //MVIY
    break;
  case '\x10':
    //MVAM <a>
    break;
  case '\x11':
    //MVXM <a>
    break;
  case '\x12':
    //MVYM <a>
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
