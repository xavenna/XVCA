#include "cpu.h"
#include "emulator.h"



void CPU::fetchInsToPCB() {
  char temp = memory.read(registers.programCounter);
  registers.PCB[registers.PCBPos] = temp;
  registers.PCBPos++;
}

void CPU::executeInstruction() {
  uint16_t address = 0;
  char temp8 = 0;
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
  case 0x15:
    //MVAM <a>
    memory.write((registers.PCB[1] << 8) & registers.PCB[2], registers.registerA);
    break;
  case 0x16:
    //MVBM <a>
    memory.write((registers.PCB[1] << 8) & registers.PCB[2], registers.registerB);
    break;
  case 0x17:
    //MVCM <a>
    memory.write((registers.PCB[1] << 8) & registers.PCB[2], registers.registerC);
    break;
  case 0x18:
    //MVXM <a>
    memory.write((registers.PCB[1] << 8) & registers.PCB[2], registers.registerX);
    break;
  case 0x19:
    //MVYM <a>
    memory.write((registers.PCB[1] << 8) & registers.PCB[2], registers.registerY);
    break;
  case 0x1a:
    //MVAI
    memory.write((registers.registerX << 8) & registers.registerY, registers.registerA);
    break;
  case 0x1b:
    //MVBI
    memory.write((registers.registerX << 8) & registers.registerY, registers.registerB);
    break;
  case 0x1c:
    //MVCI
    memory.write((registers.registerX << 8) & registers.registerY, registers.registerC);
    break;
  case 0x1d:
    //MVXI
    memory.write((registers.registerX << 8) & registers.registerY, registers.registerX);
    break;
  case 0x1e:
    //MVYI
    memory.write((registers.registerX << 8) & registers.registerY, registers.registerY);
    break;

    //////////////////////
    //  STACK OPERATIONS
    //////////////////////
  case 0x30:
    //PUSH A
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.registerA);
    break;
  case 0x31:
    //PUSH B
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.registerB);
    break;
  case 0x32:
    //PUSH C
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.registerC);
    break;
  case 0x33:
    //PUSH X
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.registerX);
    break;
  case 0x34:
    //PUSH Y
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.registerY);
    break;
  case 0x35:
    //PUSH F
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.flags.reg());
    break;
  case 0x36:
    //POP A
    registers.registerA = memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;
  case 0x37:
    //POP B
    registers.registerB = memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;
  case 0x38:
    //POP C
    registers.registerC = memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;
  case 0x39:
    //POP X
    registers.registerX = memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;
  case 0x3a:
    //POP Y
    registers.registerY = memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;
  case 0x3b:
    //POP F
    registers.flags.assignReg(memory.read(registers.stackPointer));
    registers.stackPointer++;
    break;
  case 0x3c:
    //PUSH SP
    registers.stackPointer--;
    memory.write(registers.stackPointer, (registers.stackPointer >> 8) & 0xff);
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.stackPointer & 0xff);
    break;
  case 0x3d:
    //PUSH PC
    registers.stackPointer--;
    memory.write(registers.stackPointer, (registers.programCounter >> 8) & 0xff);
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.programCounter & 0xff);
    break;
  case 0x3e:
    //POP SP
    address = memory.read(registers.stackPointer) << 8;
    registers.stackPointer++;
    address = address & memory.read(registers.stackPointer);
    registers.stackPointer++;
    registers.stackPointer = address;
    break;
  case 0x3f:
    //POP PC
    registers.programCounter = memory.read(registers.stackPointer) << 8;
    registers.stackPointer++;
    registers.programCounter = registers.stackPointer & memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;


    ////////////////////////
    // BRANCH INSTRUCTIONS
    ////////////////////////
  case 0x40:
    //JMP <a>
    registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    break;
  case 0x41:
    //JZ <a>
    if(registers.flags.zero) {
      registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    }
    break;
  case 0x42:
    //JNZ <a>
    if(!registers.flags.zero) {
      registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    }
    break;
  case 0x43:
    //JE <a>
    if(registers.flags.equal) {
      registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    }
    break;
  case 0x44:
    //JNE <a>
    if(!registers.flags.equal) {
      registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    }
    break;
  case 0x45:
    //CALL <a>
    //push programCounter to stack
    registers.programCounter++;
    registers.stackPointer--;
    memory.write(registers.stackPointer, (registers.programCounter >> 8) & 0xff);
    registers.stackPointer--;
    memory.write(registers.stackPointer, registers.programCounter & 0xff);
    //jump to argument
    registers.programCounter = (registers.PCB[1] << 8) & registers.PCB[2];
    break;
  case 0x46:
    //RET
    //pop programCounter from stack
    registers.programCounter = memory.read(registers.stackPointer) << 8;
    registers.stackPointer++;
    registers.programCounter = registers.stackPointer & memory.read(registers.stackPointer);
    registers.stackPointer++;
    break;

    ///////////////////////
    // ARITHMETIC OPCODES
    ///////////////////////

  case 0x50:
    //ADR <r>
    switch (registers.PCB[1]) {
    case 0:
      address = registers.registerA + registers.registerA;
      break;
    case 1:
      address = registers.registerA + registers.registerB;
      break;
    case 2:
      address = registers.registerA + registers.registerC;
      break;
    case 3:
      address = registers.registerA + registers.registerX;
      break;
    case 4:
      address = registers.registerA + registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x51:
    //ADCR <r>
    switch (registers.PCB[1]) {
    case 0:
      address = registers.registerA + registers.registerA + registers.flags.carry;
      break;
    case 1:
      address = registers.registerA + registers.registerB + registers.flags.carry;
      break;
    case 2:
      address = registers.registerA + registers.registerC + registers.flags.carry;
      break;
    case 3:
      address = registers.registerA + registers.registerX + registers.flags.carry;
      break;
    case 4:
      address = registers.registerA + registers.registerY + registers.flags.carry;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x52:
    //ADV <v>
    address = registers.registerA + registers.PCB[1];
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x53:
    //ADCV <v>
    address = registers.registerA + registers.PCB[1] + registers.flags.carry;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x54:
    //ADI
    address = registers.registerA + memory.read((registers.registerX << 8) & registers.registerY);
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x55:
    //ADCI
    address = registers.registerA + memory.read((registers.registerX << 8) & registers.registerY) + registers.flags.carry;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x56:
    //SBR <r>
    switch (registers.PCB[1]) {
    case 0:
      char8 = registers.registerA;
      break;
    case 1:
      char8 = registers.registerB;
      break;
    case 2:
      char8 = registers.registerC;
      break;
    case 3:
      char8 = registers.registerX;
      break;
    case 4:
      char8 = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    address = registers.registerA + !(char8) + 1;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x57:
    //SBCR <r>
    switch (registers.PCB[1]) {
    case 0:
      char8 = registers.registerA;
      break;
    case 1:
      char8 = registers.registerB;
      break;
    case 2:
      char8 = registers.registerC;
      break;
    case 3:
      char8 = registers.registerX;
      break;
    case 4:
      char8 = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    address = registers.registerA + !(char8) + 1 + registers.flags.carry;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x58:
    //SBV <v>
    address = registers.registerA + !(registers.PCB[1]) + 1;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x59:
    //SBCV <v>
    address = registers.registerA + !(registers.PCB[1]) + 1 + registers.flags.carry;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x5a:
    //SBI
    address = registers.registerA + !(memory.read((registers.PCB[1] << 8) & registers.PCB[2])) + 1;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x5b:
    //SBCI
    address = registers.registerA + !(memory.read((registers.PCB[1] << 8) & registers.PCB[2])) + 1 + registers.flags.carry;
    registers.flags.carry = (address > 0xff);
    registers.registerA = address & 0xff;
    registers.flags.zero = (registers.registerA == 0);
    registers.flags.sign = (registers.registerA & 0x80);
    break;
  case 0x5c:
    //CMPR <r>
    switch (registers.PCB[1]) {
    case 0:
      char8 = registers.registerA;
      break;
    case 1:
      char8 = registers.registerB;
      break;
    case 2:
      char8 = registers.registerC;
      break;
    case 3:
      char8 = registers.registerX;
      break;
    case 4:
      char8 = registers.registerY;
      break;
    default:
      throw std::invalid_argument("Invalid argument to instruction.");
    }
    registers.flags.equal = (char8 == registers.registerA);
    registers.flags.greater = (char8 > registers.registerA);
    registers.flags.sign = (registers.registerA-char8) & 0x80;
    break;
  case 0x5d:
    //CMPV <v>
    char8 = registers.PCB[1];
    registers.flags.equal = (char8 == registers.registerA);
    registers.flags.greater = (char8 > registers.registerA);
    registers.flags.sign = (registers.registerA-char8) & 0x80;
    break;
  case 0x5e:
    //CMPI
    char8 = memory.read((registers.registerX << 8) & registers.registerY);
    registers.flags.equal = (char8 == registers.registerA);
    registers.flags.greater = (char8 > registers.registerA);
    registers.flags.sign = (registers.registerA-char8) & 0x80;
    break;

    //////////////////////////
    //  LOGICAL INSTRUCTIONS
    //////////////////////////

    ////////////////////////////////
    //  MISCELLANEOUS INSTRUCTIONS
    ////////////////////////////////


  case 0xff:
    //HLT
    break;
  default:
    // Invalid opcode
    throw std::invalid_argument("Error: invalid opcode found");
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
