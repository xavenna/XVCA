#include "disassemble.h"


int disassemble(const std::string& file) {
  //load bin file
  std::vector<uint8_t> bin;
  std::ifstream getBoot(file);
  while(getBoot.peek() != EOF) {
    bin.push_back(getBoot.get());
  }
  //now, disassemble. Go through

  std::array<uint8_t, 4> pcb;
  uint8_t pos=0;

  std::cerr << "Disassembling:\n";
  for(int i=0;i<bin.size();i++) {
    pcb[pos] = bin[i];
    pos++;
    std::cout << std::hex;
    if(pcbIsValidIns(pcb,pos)) {
      std::cout << +(i-pos+1) << ":\t"<<std::setw(2)<<std::setfill('0');
      for(int j=0;j<pos;j++) {
        std::cout << std::setw(2) << +pcb[j];
      }
      
      std::cout << std::setw(0) << std::setfill(' ') << "\t\t";
      instructionName(pcb,pos);
      std::cout << '\n';
      pos = 0;
    }
  }

  return 1;
}

char regNum(int r) {
  //converts an int (register num) to a character
  //If the number is invalid, returns 0x0;
  char o;
  switch(r) {
    case 0:
      o = 'A';
      break;
    case 1:
      o = 'B';
      break;
    case 3:
      o = 'C';
      break;
    case 4:
      o = 'X';
      break;
    case 5: 
      o = 'Y';
      break;
    default:
      o = 0;
      break;
  }
  //std::cerr << "turning '"<<r<<"' to '"<<o<<"'\n";
  return o;
}

bool pcbIsValidIns(std::array<uint8_t,4> pcb, uint8_t pos) {
  if(pos == 0) {
    return false;
  }
  else if(pos == 1) {
    switch(pcb[0]) {
    case 0x00:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x46:
    case 0x54:
    case 0x55:
    case 0x5a:
    case 0x5b:
    case 0x72:
    case 0x75:
    case 0x78:
    case 0x7b:
    case 0xff:
      return true;
    default:
      return false;
    }
  }
  else if(pos == 2) {
    switch(pcb[0]) {
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x08:
    case 0x09:
    case 0x0a:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5c:
    case 0x5d:
    case 0x70:
    case 0x71:
    case 0x73:
    case 0x74:
    case 0x76:
    case 0x77:
    case 0x79:
    case 0x7a:
    case 0x80:
    case 0x82:
    case 0x84:
    case 0x86:
      return true;
    default:
      return false;
    }
  }
  else if(pos == 3) {
    switch(pcb[0]) {
    case 0x0b:
    case 0x0c:
    case 0x0d:
    case 0x0e:
    case 0x0f:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x88:
      return true;
    default:
      return false;
    }
  }
  return false;
}

//this all uses base 10;...bad :(
//modify to use hex
void instructionName(std::array<uint8_t,4> pcb, uint8_t pos) {
  //creates a string containing the name of the specified string.
  std::cout << std::hex << std::setfill('0') << std::setw(2);
  std::string name;
  switch(pcb[0]) {
  case 0x0:
    std::cout << "NOP";
    break;
  case 0x1:
  case 0x2:
  case 0x3:
  case 0x4:
  case 0x5:
    std::cout << "MVR" << (regNum(pcb[0]-0x1)) << " ";
    if(regNum(pcb[1]) == 0) {
      std::cout << "<INV>";
    }
    else {
      std::cout << regNum(pcb[1]);
    }
    break;
  case 0x6:
  case 0x7:
  case 0x8:
  case 0x9:
  case 0xa:
    std::cout << "MVV";
    std::cout << regNum(pcb[0]-0x6) << " " << std::setw(2) << +pcb[1];
    break;
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
    std::cout << "MVM";
    std::cout << regNum(pcb[0]-0xb) << " " <<std::setw(2) <<  +pcb[1] <<std::setw(2) <<  +pcb[2];
    break;
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
    std::cout << "MVI" << regNum(pcb[0]-0x10);
    break;
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
    std::cout << "MV";
    std::cout << regNum(pcb[0]-0x15) << "M " << std::setw(2) << +(pcb[1]) << std::setw(2) << +pcb[2];
    break;
  case 0x1a:
  case 0x1b:
  case 0x1c:
  case 0x1d:
  case 0x1e:
    std::cout << "MV";
    std::cout << regNum(pcb[0]-0x1a) << "I";
    break;

    //////////////////////
    //  STACK OPERATIONS
    //////////////////////
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
    std::cout << "PUSH " << regNum(pcb[0]-0x30);
    break;
  case 0x35:
    std::cout << "PUSH F";
    break;
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
    //POP A
    std::cout << "POP" << regNum(pcb[0]-0x36);
    break;
  case 0x3b:
    std::cout << "POP F";
    break;
  case 0x3c:
    std::cout << "PUSH SP";
    break;
  case 0x3d:
    std::cout << "PUSH PC";
    break;
  case 0x3e:
    std::cout << "POP SP";
    break;
  case 0x3f:
    std::cout << "POP PC";
    break;

    ////////////////////////
    // BRANCH INSTRUCTIONS
    ////////////////////////
  case 0x40:
    std::cout << "JMP " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x41:
    //JZ <a>
    std::cout << "JZ " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x42:
    //JNZ <a>
    std::cout << "JNZ " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x43:
    //JG <a>
    std::cout << "JG " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x44:
    //JNG <a>
    std::cout << "JNG " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x45:
    //CALL <a>
    std::cout << "CALL " << std::setw(4) << (int(pcb[1]) << 8 | pcb[2]);
    break;
  case 0x46:
    std::cout << "RET";
    break;

    ///////////////////////
    // ARITHMETIC OPCODES
    ///////////////////////

  case 0x50:
    std::cout << "ADR " << regNum(pcb[1]);
    break;
  case 0x51:
    std::cout << "ADCR " << regNum(pcb[1]);
    break;
  case 0x52:
    std::cout << "ADV " << std::setw(2) << +pcb[1];
    break;
  case 0x53:
    std::cout << "ADCV " << std::setw(2) << +pcb[1];
    break;
  case 0x54:
    std::cout << "ADI";
    break;
  case 0x55:
    std::cout << "ADCI";
    break;
  case 0x56:
    std::cout << "SBR " << regNum(pcb[1]);
    break;
  case 0x57:
    std::cout << "SBCR " << regNum(pcb[1]);
    break;
  case 0x58:
    std::cout << "SBV " << std::setw(2) << +pcb[1];
    break;
  case 0x59:
    std::cout << "SBCV " << std::setw(2) << +pcb[1];
    break;
  case 0x5a:
    std::cout << "SBI";
    break;
  case 0x5b:
    std::cout << "SBCI";
    break;
  case 0x5c:
    std::cout << "CMPR " << regNum(pcb[1]);
    break;
  case 0x5d:
    std::cout << "CMPV " << std::setw(2) << +pcb[1];
    break;
  case 0x5e:
    std::cout << "CMPI";
    break;

    //////////////////////////
    //  LOGICAL INSTRUCTIONS
    //////////////////////////

  case 0x70:
    std::cout << "ORR " << regNum(pcb[1]);
    break;
  case 0x71:
    std::cout << "ORV " << std::setw(2) << +pcb[1];
    break;
  case 0x72:
    std::cout << "ORI";
    break;
  case 0x73:
    std::cout << "ANDR " << regNum(pcb[1]);
    break;
  case 0x74:
    std::cout << "ANDV " << std::setw(2) << +pcb[1];
    break;
  case 0x75:
    std::cout << "ANDI";
    break;
  case 0x76:
    std::cout << "XORR " << regNum(pcb[1]);
    break;
  case 0x77:
    std::cout << "XORV " << std::setw(2) << +pcb[1];
    break;
  case 0x78:
    std::cout << "XORI";
    break;
  case 0x79:
    std::cout << "NOTR " << regNum(pcb[1]);
    break;
  case 0x7a:
    std::cout << "NOTV " << std::setw(2) << +pcb[1];
    break;
  case 0x7b:
    std::cout << "NOTI";
    break;
    
    ////////////////////////////////
    //  MISCELLANEOUS INSTRUCTIONS
    ////////////////////////////////

  case 0x80:
    //name = std::string("SPI ") + std::to_string(+pcb[1]);
    std::cout << "SPI " << std::setw(2) << +pcb[1];
    break;
  case 0x81:
    std::cout << "SPIA";
    break;
  case 0x82:
    //name = std::string("SPD ") + std::to_string(+pcb[1]);
    std::cout << "SPD " << std::setw(2) << +pcb[1];
    break;
  case 0x83:
    std::cout << "SPDA";
    break;
  case 0x84:
    std::cout << "PCI " << std::setw(2) << +pcb[1];
    //name = std::string("PCI ") + std::to_string(+pcb[1]);
    break;
  case 0x85:
    std::cout << "PCIA";
    break;
  case 0x86:
    //std::cout << std::string("PCD ") << +pcb[1];
    std::cout << "PCD " << std::setw(2) << +pcb[1];
    break;
  case 0x87:
    std::cout << "PCDA";
    break;
  case 0x88:
    //SPS <a>
    std::cout << "SPS " << std::setw(4) << (+(pcb[1] << 8) | pcb[2]);
    break;

    //No instructions exist in this gap

  case 0xff:
    std::cout << "HLT";
    break;
  default:
    std::cout << "INVALID";
  }
  //return name;
}
