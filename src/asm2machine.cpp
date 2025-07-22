#include "assembler-util.h"
//this file contains the transformLineToMachineCode function.

//I really need to organize this.
//First, split line at spaces to get an arg list. Make helper functions as needed
int transformLineToMachineCode(std::vector<char>& machineCode, std::string line, std::map<int, std::string>& jumpHash, int lineNum) {
  //check if line is an instruction or a directive (like .db)
  size_t beginSize = machineCode.size();  //used for creating label offsets 
  std::string instName = line.substr(0, line.find(" "));

  std::vector<std::string> args;
  split(line, args);
  int argNum = args.size();

  if(line[0] == '.') {
    //resolve directive
    if(instName == ".db") {
      //insert a single raw byte into the machine code
      if(argNum != 2) { 
        std::cout << "Error: missing argument at line " << lineNum << ".\n";
      }
      else {
        try {
          int arg = rewriteNum(args[1]);
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
    else if(instName == ".dbchar") {
      //insert a single ascii char into the machine code
      if(line.size() != 9) {
        std::cout << "Error: argument error at line "<<lineNum << ".\n";
      }
      else {
        machineCode.push_back(line[8]);
      }
    }
    else if(instName == ".string") {
      //insert a series of ascii chars into the machine code
      if(argNum != 2) {
        std::cerr << "Error: argument error at line "<<lineNum << ".\n";
      } else {
        if(args[1].size() > 2) {
          for(unsigned i=1; i<(args[1].size()-1); i++) {
            machineCode.push_back(args[1][i]);
          }
          machineCode.push_back(0);
        }
      }
    }
    return (machineCode.size() - beginSize);  //number of bytes added
  }
  //the following section is kind of an unreasonable mess. Something could probably be done about this
  //maybe parse arguments at the beginning, then each function can deal with them

  //enumerate all instruction bases here, so they can be handled differently
  if(instName == "MVRA") {  
    //move specified register to register A
    if(argNum != 2) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
    }
    else {
      machineCode.push_back(0x01);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument at line " << lineNum << ".\n";
      }
    }
  }
  else if(instName == "MVRB") {
    //move specified register to register B
    if(argNum != 2) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
    }
    else {
      machineCode.push_back(0x02);
      if(isReg(args[1][0])) {
        machineCode.push_back(regToNum(args[1][0]));
      }
      else {
        std::cout << "Error: invalid argument at line " << lineNum << ".\n";
      }
    }
  }
  else if(instName == "MVRC") {
    //move specified register to register C
    if(line.size() < 6) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
    }
    else {
      machineCode.push_back(0x03);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument at line " << lineNum << ".\n";
      }
    }
  }
  else if(instName == "MVRX") {
    //move specified register to register X
    if(line.size() < 6) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
    }
    else {
      machineCode.push_back(0x04);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument at line " << lineNum << ".\n";
      }
    }
  }
  else if(instName == "MVRY") {
    //move specified register to register Y
    if(line.size() < 6) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
    }
    else {
      machineCode.push_back(0x05);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument at line " << lineNum << ".\n";
      }
    }
  }
  else if(instName == "MVVA") {
    machineCode.push_back(0x06);
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
  else if(instName == "MVVB") {
    machineCode.push_back(0x07);
    if(line.size() < 6) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
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
  else if(instName == "MVVC") {
    machineCode.push_back(0x08);
    if(line.size() < 6) {
      std::cout << "Error: missing argument at line " << lineNum << ".\n";
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
  else if(instName == "MVVX") {
    machineCode.push_back(0x09);
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
    machineCode.push_back(0x0a);
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
    machineCode.push_back(0x0b);
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
  else if(instName == "MVMB") {
    machineCode.push_back(0x0c);
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
  else if(instName == "MVMC") {
    machineCode.push_back(0x0d);
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
    machineCode.push_back(0x0e);
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
    machineCode.push_back(0x0f);
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
    machineCode.push_back(0x10);
  }
  else if(instName == "MVIB") {
    machineCode.push_back(0x11);
  }
  else if(instName == "MVIC") {
    machineCode.push_back(0x12);
  }
  else if(instName == "MVIX") {
    machineCode.push_back(0x13);
  }
  else if(instName == "MVIY") {
    machineCode.push_back(0x14);
  }
  else if(instName == "MVAM") {
    machineCode.push_back(0x15);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MVBM") {
    machineCode.push_back(0x16);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MVCM") {
    machineCode.push_back(0x17);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MVXM") {
    machineCode.push_back(0x18);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MVYM") {
    machineCode.push_back(0x19);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MVAI") {
    machineCode.push_back(0x1a);
  }
  else if(instName == "MVBI") {
    machineCode.push_back(0x1b);
  }
  else if(instName == "MVCI") {
    machineCode.push_back(0x1c);
  }
  else if(instName == "MVXI") {
    machineCode.push_back(0x1d);
  }
  else if(instName == "MVYI") {
    machineCode.push_back(0x1e);
  }
  else if(instName == "MIXY") {
    machineCode.push_back(0x1f);
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
        //target is label
        int targetLocation = machineCode.size();
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 6)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "MSXY") {
    machineCode.push_back(0x20);
  }
  else if(instName == "MXYS") {
    machineCode.push_back(0x21);
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
      else if(line[5] == 'B') {
        machineCode.push_back(0x31);
      }
      else if(line[5] == 'C') {
        machineCode.push_back(0x32);
      }
      else if(line[5] == 'X') {
        machineCode.push_back(0x33);
      }
      else if(line[5] == 'Y') {
        machineCode.push_back(0x34);
      }
      else if(line[5] == 'F') {
        machineCode.push_back(0x35);
      }
      else if(line.size() > 6 && line[5] == 'S' && line[6] == 'P') {
        machineCode.push_back(0x3c);
      }
      else if(line.size() > 6 && line[5] == 'P' && line[6] == 'C') {
        machineCode.push_back(0x3d);
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
        machineCode.push_back(0x36);
      }
      else if(line[4] == 'B') {
        machineCode.push_back(0x37);
      }
      else if(line[4] == 'C') {
        machineCode.push_back(0x38);
      }
      else if(line[4] == 'X') {
        machineCode.push_back(0x39);
      }
      else if(line[4] == 'Y') {
        machineCode.push_back(0x3a);
      }
      else if(line[4] == 'F') {
        machineCode.push_back(0x3b);
      }
      else if(line.size() > 6 && line[5] == 'S' && line[6] == 'P') {
        machineCode.push_back(0x3e);
      }
      else if(line.size() > 6 && line[5] == 'P' && line[6] == 'C') {
        machineCode.push_back(0x3f);
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
  else if(instName == "JG") {
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
  else if(instName == "JNG") {
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
  else if(instName == "CALL") {
    machineCode.push_back(0x45);
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
        jumpHash.emplace(targetLocation, line.substr(5, line.find(" ", 5)));
        machineCode.push_back(0x00);
        machineCode.push_back(0x00);
      }
    }
  }
  else if(instName == "RET") {
    machineCode.push_back(0x46);
  }
  else if(instName == "ADR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x50);
      if(isReg(line[4])) {
        machineCode.push_back(regToNum(line[4]));
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
      machineCode.push_back(0x51);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ADV") {
    machineCode.push_back(0x52);
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
    machineCode.push_back(0x53);
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
    machineCode.push_back(0x54);
  }
  else if(instName == "ADCI") {
    machineCode.push_back(0x55);
  }
  else if(instName == "SBR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x56);
      if(isReg(line[4])) {
        machineCode.push_back(regToNum(line[4]));
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
      machineCode.push_back(0x57);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "SBV") {
    machineCode.push_back(0x58);
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
    machineCode.push_back(0x59);
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
    machineCode.push_back(0x5a);
  }
  else if(instName == "SBCI") {
    machineCode.push_back(0x5b);
  }
  else if(instName == "CMPR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x5c);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "CMPV") {
    machineCode.push_back(0x5d);
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid 8-bit value
      try {
        int arg = rewriteNum(line.substr(5, line.find(" ", 6)));
        if(arg > 255 || arg < 0) {
          std::cout << "Error: argument is not an unsigned 8 bit integer\n";
          return -1;
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
  else if(instName == "CMPI") {
    machineCode.push_back(0x5e);
  }

  else if(instName == "ORR") {
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x70);
      if(isReg(line[4])) {
        machineCode.push_back(regToNum(line[4]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ORV") {
    machineCode.push_back(0x71);
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
    machineCode.push_back(0x72);
  }
  else if(instName == "ANDR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x73);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "ANDV") {
    machineCode.push_back(0x74);
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
    machineCode.push_back(0x75);
  }
  else if(instName == "XORR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x76);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "XORV") {
    machineCode.push_back(0x77);
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
    machineCode.push_back(0x78);
  }
  else if(instName == "NOTR") {
    if(line.size() < 6) {
      std::cout << "Error: missing argument for instruction '" << instName << "'\n";
    }
    else {
      machineCode.push_back(0x79);
      if(isReg(line[5])) {
        machineCode.push_back(regToNum(line[5]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
      }
    }
  }
  else if(instName == "NOTV") {
    machineCode.push_back(0x7a);
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
  else if(instName == "NOTI") {
    machineCode.push_back(0x7b);
  }
  else if(instName == "SPI") {
    machineCode.push_back(0x80);
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
    machineCode.push_back(0x81);
  }
  else if(instName == "SPD") {
    machineCode.push_back(0x82);
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
    machineCode.push_back(0x83);
  }
  else if(instName == "PCI") {
    machineCode.push_back(0x84);
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
    machineCode.push_back(0x85);
  }
  else if(instName == "PCD") {
    machineCode.push_back(0x86);
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
    machineCode.push_back(0x87);
  }
  else if(instName == "SPS") {
    machineCode.push_back(0x88);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //address
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
  else if(instName == "INC") {
    machineCode.push_back(0x89);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid register
      if(isReg(line[4])) {
        machineCode.push_back(regToNum(line[4]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
        return -1;
      }
    }
  }
  else if(instName == "DEC") {
    machineCode.push_back(0x8a);
    if(line.size() < 5) {
      std::cout << "Error: missing argument for instruction " << instName << " \n";
      return -1;
    }
    else {  //check if argument is a valid register
      if(isReg(line[4])) {
        machineCode.push_back(regToNum(line[4]));
      }
      else {
        std::cout << "Error: invalid argument for instruction '" << instName << "'\n";
        return -1;
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
    std::cout << "Error: invalid instruction '" << instName << "' at line " << lineNum << ".\n";
    return -1;
  }
  return (machineCode.size() - beginSize);  //number of bytes added
}
