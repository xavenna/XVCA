#include "assembler-util.h"
#include "util.h"
#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <cstdint>




int rewriteNum(std::string n) {
  if(n.substr(0,1) == "$") {
    //number is hex
    return std::stoi (n.substr(1),nullptr,16);
  }
  else if(n.substr(0,1) == "%") {
    //number is binary
    return std::stoi (n.substr(1),nullptr,2);
  }
  else {
    //number is decimal
    return std::stoi (n);
  }
  throw -1;  //this should never be reached. TBH, I have no idea why it is here
}

void createHeader(std::vector<char>& machineCode, std::vector<char>& machineCodeWithHeader, uint16_t jp) {
  //std::cout << "jp: " << jp << '\n';
  char jpH = ((jp & 0xff00) >> 8) & 0xff;
  char jpL = jp & 0xff;
  char temp[16]{'X','V','C','A',0x10,0x00, jpH, jpL, '\0','\0','\0','\0','\0','\0','\0','\0'};
  machineCodeWithHeader.resize(16 + machineCode.size());
  memcpy(&(*machineCodeWithHeader.begin()), temp, 16);
  memcpy(&(*(machineCodeWithHeader.begin()+16)), &(*machineCode.begin()), machineCode.size());

}

int transformLineToMachineCode(std::vector<char>& machineCode, std::string line, std::map<int, std::string>& jumpHash, int lineNum) {  //maybe move this function to a new file, as it is over 1000 lines of code
  //std::cout << line << ',' << line.size() << '\n';
  size_t beginSize = machineCode.size();  //used for creating label offsets 
  line = line.substr(0,line.find(";")); //trim comments
  std::string instName = line.substr(0, line.find(" "));
  //the following section is kind of an unreasonable mess. Something could probably be done about this
  if(instName == "MVRA") {  //enumerate all instruction bases here, so they can be handled differently
    //move specified register to register A
    if(line.size() < 6) {
      //error: no argument
    }
    else {
      if(line[5] == 'X') {
	//0x01
	machineCode.push_back(0x01);
      }
      else if(line[5] == 'Y') {
	//0x02
	machineCode.push_back(0x02);
      }
      else {
	//error: invalid argument
      }
    }
  }
  else if(instName == "MVRX") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x03);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x04);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "MVRY") {
    //move specified register to register Z
    if(line.size() < 6) {
      //error: no argument
    }
    else {
      if(line[5] == 'A') {
	//0x01
	machineCode.push_back('\x05');
      }
      else if(line[5] == 'X') {
	//0x02
	machineCode.push_back('\x06');
      }
      else {
	//error: invalid argument
      }
    }
  }
  else if(instName == "MVVA") {
    machineCode.push_back('\x07');
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVVX") {
    machineCode.push_back('\x08');
    if(line.size() < 6) {
      //error: missing argument
    }
    else {
      //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	//std::cout << arg << '\n';
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	  //error: argument is not an unsigned 8-bit integer
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVVY") {
    machineCode.push_back('\x09');
    if(line.size() < 6) {
      //error: missing argument
    }
    else {
      //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	//std::cout << arg << '\n';
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVMA") {
    machineCode.push_back('\x0a');
    if(line.size() < 6) {
      //error: missing argument
    }
    else {
      //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVMX") {
    machineCode.push_back('\x0b');
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVMY") {
    machineCode.push_back('\x0c');
    if(line.size() < 6) {
      //error: missing argument
    }
    else {
      //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVIA") {
    machineCode.push_back(0x0d);
  }
  else if(instName == "MVIX") {
    machineCode.push_back(0x0e);
  }
  else if(instName == "MVIY") {
    machineCode.push_back(0x0f);
  }
  else if(instName == "MVAM") {
    machineCode.push_back(0x10);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVXM") {
    machineCode.push_back(0x11);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVYM") {
    machineCode.push_back(0x12);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "MVAI") {
    machineCode.push_back(0x13);
  }
  else if(instName == "MVXI") {
    machineCode.push_back(0x14);
  }
  else if(instName == "MVYI") {
    machineCode.push_back(0x15);
  }
  else if(instName == "MVVM") {
    //maybe this shouldn't be part of the instruction set
    //because this would be annoying to code
    std::cout << "The MVVM Instruction has not yet been implemented. Please avoid using it for now.\n";
    return -1;
  }  
  else if(instName == "PUSH") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x30);
      }
      else if(line[5] == 'X') {
	machineCode.push_back(0x31);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x32);
      }
      else if(line[5] == 'F') {
	machineCode.push_back(0x33);
      }
      else if(line.size() > 6 && line[5] == 'P' && line[6] == 'C') {
	machineCode.push_back(0x38);
      }
      else if(line.size() > 6 && line[5] == 'S' && line[6] == 'P') {
	machineCode.push_back(0x39);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "POP") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'A') {
	machineCode.push_back(0x34);
      }
      else if(line[4] == 'X') {
	machineCode.push_back(0x35);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x36);
      }
      else if(line[4] == 'F') {
	machineCode.push_back(0x37);
      }
      else if(line.size() > 6 && line[5] == 'P' && line[6] == 'C') {
	machineCode.push_back(0x3a);
      }
      else if(line.size() > 6 && line[5] == 'S' && line[6] == 'P') {
	machineCode.push_back(0x3b);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "JMP") {
    machineCode.push_back(0x40);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	//target is label
	int targetLocation = machineCode.size();
	jumpHash.emplace(targetLocation, line.substr(4, line.find(" ", 5)));
	machineCode.push_back(0x00);
	machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "JZ") {
    machineCode.push_back(0x41);
    if(line.size() < 4) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(3, line.find(" ", 4)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	int targetLocation = machineCode.size();
	jumpHash.emplace(targetLocation, line.substr(3, line.find(" ", 4)));
	machineCode.push_back(0x00);
	machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "JNZ") {
    machineCode.push_back(0x42);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	int targetLocation = machineCode.size();
	jumpHash.emplace(targetLocation, line.substr(4, line.find(" ", 5)));
	machineCode.push_back(0x00);
	machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "JE") {
    machineCode.push_back(0x43);
    if(line.size() < 4) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(3, line.find(" ", 4)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	int targetLocation = machineCode.size();
	jumpHash.emplace(targetLocation, line.substr(3, line.find(" ", 4)));
	machineCode.push_back(0x00);
	machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "JNE") {
    machineCode.push_back(0x44);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
      return -1;
    }
    else {
      try {  //check if argument is a valid 8-bit value
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 0xffff || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 16 bit integer\n";
	}
	else {
	  machineCode.push_back((arg & 0xff00) >> 8);
	  machineCode.push_back(arg & 0x00ff);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix.\n";
	}
      }
      catch (...) {
	int targetLocation = machineCode.size();
	jumpHash.emplace(targetLocation, line.substr(4, line.find(" ", 5)));
	machineCode.push_back(0x00);
	machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "RET") {
    machineCode.push_back(0x4f);
  }
  else if(instName == "ADR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'A') {
	machineCode.push_back(0x50);
      }
      else if(line[4] == 'X') {
	machineCode.push_back(0x51);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x52);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ADCR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x53);
      }
      else if(line[5] == 'X') {
	machineCode.push_back(0x54);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x55);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ADV") {
    machineCode.push_back(0x56);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "ADCV") {
    machineCode.push_back(0x57);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
    
  }
  else if(instName == "ADI") {
    machineCode.push_back(0x58);
  }
  else if(instName == "ADCI") {
    machineCode.push_back(0x59);
  }
  else if(instName == "SBR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'A') {
	machineCode.push_back(0x5a);
      }
      else if(line[4] == 'X') {
	machineCode.push_back(0x5b);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x5c);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "SBCR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x5d);
      }
      else if(line[5] == 'X') {
	machineCode.push_back(0x5e);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x5f);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "SBV") {
    machineCode.push_back(0x60);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "SBCV") {
    machineCode.push_back(0x61);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "SBI") {
    machineCode.push_back(0x62);
  }
  else if(instName == "SBCI") {
    machineCode.push_back(0x63);
  }
  else if(instName == "CMP") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'X') {
	machineCode.push_back(0x64);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x65);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ORR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'A') {
	machineCode.push_back(0x70);
      }
      else if(line[4] == 'X') {
	machineCode.push_back(0x71);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x72);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ORV") {
    machineCode.push_back(0x73);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "ORI") {
    machineCode.push_back(0x74);
  }
  else if(instName == "ANDR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x75);
      }
      else if(line[5] == 'X') {
	machineCode.push_back(0x76);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x77);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ANDV") {
    machineCode.push_back(0x78);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "ANDI") {
    machineCode.push_back(0x79);
  }
  else if(instName == "XORR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[5] == 'A') {
	machineCode.push_back(0x7a);
      }
      else if(line[5] == 'X') {
	machineCode.push_back(0x7b);
      }
      else if(line[5] == 'Y') {
	machineCode.push_back(0x7c);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "XORV") {
    machineCode.push_back(0x7d);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "XORI") {
    machineCode.push_back(0x7e);
  }
  else if(instName == "NOT") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      if(line[4] == 'A') {
	machineCode.push_back(0x7f);
      }
      else if(line[4] == 'X') {
	machineCode.push_back(0x80);
      }
      else if(line[4] == 'Y') {
	machineCode.push_back(0x81);
      }
      else {
	std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "SPI") {
    machineCode.push_back(0x88);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "SPIA") {
    machineCode.push_back(0x89);
  }
  else if(instName == "SPD") {
    machineCode.push_back(0x8a);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "SPDA") {
    machineCode.push_back(0x8b);
  }
  else if(instName == "PCI") {
    machineCode.push_back(0x8c);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "PCIA") {
    machineCode.push_back(0x8d);
  }
  else if(instName == "PCD") {
    machineCode.push_back(0x8e);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "PCDA") {
    machineCode.push_back(0x8f);
  }
  else if(instName == "SPS") {
    machineCode.push_back(0x90);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
	int arg = rewriteNum(line.substr(4, line.find(" ", 5)));
	if(arg > 255 || arg < 0) {
	  std::cout << "Error: argument is not an unsigned 8 bit integer\n";
	}
	else {
	  machineCode.push_back(arg);
	}
      }
      catch (int e) {
	if(e == -1) {
	  std::cout << "Error: invalid numeric prefix";
	  //error: ...
	}
      }
      catch (...) {
	std::cout << "Error: argument could not be parsed as a number\n";
      }
    }
  }
  else if(instName == "NOP") {
    machineCode.push_back(0x00);
  }
  else if(instName == "HLT") {
    machineCode.push_back(0xff);
  }
  else {
    std::cout << "Error: invalid instruction '" << instName << "'.\n";
    return -1;
  }
  return (machineCode.size() - beginSize);  //number of instructions added
}

bool writeMachineCodeToFile(std::vector<char>& machineCode, std::string filename) {
  size_t len = machineCode.size();
  std::ofstream put(filename, std::ios::trunc | std::ios::binary);
  if(!put.is_open()) {
    return false;
  }
  char* buffer = new char[len];
  memcpy(buffer, &(*machineCode.begin()), len);
  put.write(buffer, len);
  delete[] buffer;
  return true;
}

bool fixLabelJumpPoints(std::vector<char>& machineCode, std::map<std::string, int>& labelHash, std::map<int, std::string> jumpTable, std::vector<int> codesPerLine) {
  //machineCode contains the unfinished machine code for the program.
  //labelHash contains a list of which line of code each label corresponds to
  //jumpTable contains a list of where a label is needed (in machine code), and which label is needed there
  //codesPerLine tells how many bytes of machine code each line was translated into
  

  //first, construct a list of the machine code positions of each line of code
  std::vector<int> cumulativePosition;  // machine code position of each line of code
  int accum = 0;
  for(auto x : codesPerLine) {  //make sure this actually works
    cumulativePosition.push_back(accum);
    accum += x;
  }
  cumulativePosition.push_back(accum);

  //second, create a list of where each label is located in machine code, rather than lines of code
  for(auto& x : labelHash) {
    x.second = cumulativePosition.at(x.second);
  }

  //next, iterate through jumpTable to find each place where an address is needed.
  //  Find where that label is located in machine code, and write the address to the placeholder space after the instruction
  try {
    for(auto x : jumpTable) {
      //x.second is label name
      //labelHash[x.second] is label position
      //add label position to starting offset, and write to machineCode[x.first]
      if(labelHash.find(x.second) == labelHash.end()) {
	std::cout << "Error: Attempt to jump to nonexistant label.\n";
	return false;
      }
      int relativeAddress = labelHash.find(x.second)->second;  //add exception handling here, as an undefined label will currently cause a crash
      int absoluteAddress = relativeAddress + 0x1000;  //0x1000 is the program loading point, and thus the location of the first byte of machine code
      machineCode[x.first] = (absoluteAddress & 0xff00) >> 8;
      machineCode[x.first+1] = absoluteAddress & 0xff;
      
      if(labelHash.find(x.second) == labelHash.end()) {
	std::cout << "Error: couldn't find label\n";
      }
    }
  }
  catch (...) {
    std::cout << "Error: invalid label. (or a bug. If you know you didn't make any label mistakes, please contact the developer)\n";
    return false;
  }
  
  //I think that is all
  return true;
} 
