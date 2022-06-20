#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "util.h"
#include "assembler.h"

/* This is the main file for the XVCA Assembler. Eventually, the goal is to make an assembler that runs on XVCA
 * For now, though, it is written in c++
 * This version targets the XVCA-P1.1r version of the instruction set

 * Arguments:
 * -h: display help
 */


int main(int argc, char** argv) {
  //parse arguments
  if(argc == 1) {
    std::cout << "Error: No arguments supplied. Use '" << argv[0] << " -h' for help\n";
    return 1;
  }
  else if(argc == 2) {
    if(std::string(argv[1]) == "-h") {
      //display help message
    }
    else {
      //attempt to assemble second argument
      if(!assemble(argv[1])) {
	//error
      }
      else {
	//success
      }
    }
  }
}

bool assemble(std::string filename) {
  std::ifstream get(filename);
  std::string line;
  std::vector<std::string> lines;  //all lines in program
  std::vector<char> machineCode;  //I know that a vector of chars is basically a string, but it feels wrong to store a series of numbers in a string...
  std::map<std::string, int> labelHash;
  if(!get.is_open()) {
    return false;
  }
  int counter = 0;
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() <= 1) {
      //line is empty, disregard
    }
    else if(isLetter(line.at(0))) {  //uses at rather than [] so I get an exception. Maybe add try catch?
      //it is a label
      labelHash.emplace(line.substr(0,line.find(':')), counter);
    }
    else if(line.substr(1,1) == ";") {
      //line is a comment, disregard
    }
    else {
      lines.push_back(line.substr(1));  //each instruction line begins with a tab, which is trimmed here
      counter++;  //counter is only incremented if a label isn't found so labels point to the following line
    }
  }
  //now transform each line into the corresponding machine code instruction(s)
  for(size_t i=0;i<lines.size();i++) {
    //std::cout << "parsing line '" << lines[i] << "'\n";
    lines[i] = lines[i].substr(0,lines[i].find(";")); //trim comments
    std::string instName = lines[i].substr(0, lines[i].find(" "));
    //the following section is kind of an unreasonable mess. Something could probably be done about this
    if(instName == "MVRA") {  //enumerate all instruction bases here, so they can be handled differently
      //move specified register to register A
      if(lines[i].size() < 6) {
	//error: no argument
      }
      else {
	if(lines[i][5] == 'X') {
	  //0x01
	  machineCode.push_back('\x01');
	}
	else if(lines[i][5] == 'Y') {
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
      if(lines[i].size() < 6) {
	//error: no argument
      }
      else {
	if(lines[i][5] == 'A') {
	  //0x01
	  machineCode.push_back('\x03');
	}
	else if(lines[i][5] == 'Y') {
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
      if(lines[i].size() < 6) {
	//error: no argument
      }
      else {
	if(lines[i][5] == 'A') {
	  //0x01
	  machineCode.push_back('\x05');
	}
	else if(lines[i][5] == 'X') {
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
      if(lines[i].size() < 6) {
	//error: missing argument
      }
      else {
	//check if argument is a valid 8-bit value
	try {
	  int arg = rewriteNum(lines[i].substr(5, lines[i].find(" ", 6)));
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
    //parse line, remove comments, transform into machine code
  }
  //for debugging, print the contents of machineCode
  for(auto x : machineCode) {
    std::cout << HEX( x ) << '\n';
  }
  return true;  //so g++ doesn't get mad at me, remove once the function is finished
}
