#include "assembler-util.h"
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
  else {
    //number is decimal
    return std::stoi (n);
  }
  throw -1;
}

void createHeader(std::vector<char>& machineCode, std::vector<char>& machineCodeWithHeader, uint16_t jp) {
  std::cout << "jp: " << jp << '\n';
  char jpH = ((jp & 0xff00) >> 8) & 0xff;
  char jpL = jp & 0xff;
  char temp[16]{'X','V','C','A',0x10,0x00, jpH, jpL, '\0','\0','\0','\0','\0','\0','\0','\0'};
  std::cout << machineCode.size() << '\n';
  machineCodeWithHeader.resize(16 + machineCode.size());
  std::cout << machineCodeWithHeader.size() << '\n';
  memcpy(&(*machineCodeWithHeader.begin()), temp, 16);
  memcpy(&(*(machineCodeWithHeader.begin()+16)), &(*machineCode.begin()), machineCode.size());

}

int transformLineToMachineCode(std::vector<char>& machineCode, std::string line, std::map<int, std::string>& jumpHash) {
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
	machineCode.push_back('\x01');
      }
      else if(line[5] == 'Y') {
	//0x02
	machineCode.push_back('\x02');
      }
      else {
	//error: invalid argument
      }
    }
  }
  else if(instName == "MVRX") {
    //move specified register to register X
    if(line.size() < 6) {
      //error: no argument
    }
    else {
      if(line[5] == 'A') {
	//0x01
	machineCode.push_back('\x03');
      }
      else if(line[5] == 'Y') {
	//0x02
	machineCode.push_back('\x04');
      }
      else {
	//error: invalid argument
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
	//error: argument could not be parsed as a number
	std::cout << "Error: argument could not be parsed as a number";
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
	//error: argument could not be parsed as a number
	std::cout << "Error: argument could not be parsed as a number";
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
	std::cout << "Error: argument could not be parsed as a number";
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
	std::cout << "Error: argument could not be parsed as a number";
      }
    }
  }
  else if(instName == "MVMX") {
    machineCode.push_back('\x0b');
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
	std::cout << "Error: argument could not be parsed as a number";
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
	std::cout << "Error: argument could not be parsed as a number";
      }
    }
  }
  else {
    //error: invalid instruction
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

  //second, create a list of where each label is located in machine code, rather than lines of code
  for(auto& x : labelHash) {
    x.second = cumulativePosition[x.second];  //make sure this actually works
  }

  //next, iterate through jumpTable to find each place where an address is needed.
  //  Find where that label is located in machine code, and write the address to the placeholder space after the instruction
  try {
    for(auto x : jumpTable) {
      //x.second() is label name
      //labelHash[x.second()] is label position
      //add label position to starting offset, and write to machineCode[x.first()]
      int relativeAddress = labelHash[x.second];  //add exception handling here, as an undefined label will currently cause a crash
      int absoluteAddress = relativeAddress + 0x1000;  //0x1000 is the program loading point, and thus the location of the first byte of machine code
      machineCode[x.first] = (absoluteAddress & 0xff00) >> 8;
      machineCode[x.first+1] = absoluteAddress & 0xff;
      
    }
  }
  catch (...) {
    std::cout << "Error: invalid label. (or a bug. If you didn't make any label mistakes, please contact the developer)\n";
    return false;
  }
  
  //I think that is all
  return true;
} 
