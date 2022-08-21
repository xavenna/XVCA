#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "util.h"
#include "assembler.h"
#include "assembler-util.h"

/* This is the main file for the XVCA Assembler. Eventually, the goal is to make an assembler that runs on XVCA
 * For now, though, it is written in c++
 * This version targets the XVCA-P2 version of the instruction set

 * Arguments:
 * -h: display help
 */


int main(int argc, char** argv) {
  //parse arguments
  std::string infile;
  std::string outfile;
  unsigned jumpPoint = 0x1000;
  bool generateJumpTable = false;
  bool help = false;
  bool makeHeader = true;
  if(argc == 1) {
    std::cout << "Error: No arguments supplied. Use '" << argv[0] << " -h' for help\n";
    return 1;
  }
  for(int i=1;i<argc;i++) {
    std::string arg(argv[i]);

    if(arg.substr(0,2) == "-i") {
      //inputfile
      if(arg.size() > 2) {
	infile = arg.substr(2);
      }
      else {
	std::cout << "Error: flag '-i' requires an argument.\n";
	return 1;
      }
    }
    else if(arg.substr(0,2) == "-o") {
      //outfile
      if(arg.size() > 2) {
	outfile = arg.substr(2);
      }
      else {
	std::cout << "Error: flag '-o' requires an argument.\n";
	return 1;
      }
    }
    else if(arg.substr(0,2) == "-j") {
      //jump point
      if(arg.size() > 2) {
	try {
	  jumpPoint = std::stoul(arg.substr(2));
	}
	catch (...) {
	  std::cout << "Error: flag '-j' requires a numeric argument.\n";
	  return 1;
	}
      }
      else {
	std::cout << "Error: flag '-j' requires an argument.\n";
	return 1;
      }
    }
    else if(arg == "-g") {
      //generate a jump table
      generateJumpTable = true;
    }
    else if(arg == "-n") {
      //don't create a header
      makeHeader = false;
    }
    else if(arg == "-h") {
      //display help
      help = true;
    }
    else {
      std::cout << "Error: unrecognized argument '" << arg << "'\n";
      return 1;
    }
    
  }
  //now, do things
  if(help) {
    //display help
    std::cout << "XVCA Assembler: version Test0.0\n";
    std::cout << "Usage: " << argv[0] << " <input file> <output file>\n";
    std::cout << "TODO: Update help menu, it is outdated\n";
  }
  else if(!infile.empty() && !outfile.empty()) {
    //assemble
    if(!assemble(infile, outfile, jumpPoint, generateJumpTable, makeHeader)) {
      std::cout << "Assembly failed.\n";
      return 1;
    }
  }
  else {
    std::cout << "Error: invalid command. Use '" << argv[0] << " -h' to see help.\n";
  }
}

bool assemble(std::string filename, std::string outfile, unsigned addrOff, bool makeJumpTable, bool header) {
  std::ifstream get(filename);
  std::string line;
  std::vector<std::string> lines;  //all lines in program
  std::vector<char> machineCode;  //I know that a vector of chars is basically a string, but it feels wrong to store a series of numbers in a string...
  std::vector<int> codesPerLine;
  std::map<std::string, int> labelHash;
  std::map<int, std::string> jumpHash;
  if(!get.is_open()) {
    std::cout << "Error: file not found.\n";
    return false;
  }
  //prepare header
  int counter = 0;
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line == "#NOHEADER") {
      std::cout << "Error: the noheader directive is no longer supported. Use the '-n' flag instead.\n";
      return false;
    }
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
    int value = transformLineToMachineCode(machineCode, lines[i], jumpHash, i);
    if(value < 0) {
      //error:
      std::cout << "Error: instruction not turned to machine code\n";
      return false;
    }
    codesPerLine.push_back(value);
  }

  fixLabelJumpPoints(machineCode, labelHash, jumpHash, codesPerLine, addrOff);
  //TODO: Make fixLabelJumpPoints able to create a label table (e.g. for syscalls or driver functions)

  //jumpHash: {address location in bytecode, target label}
  auto jumpPoint = labelHash.find("begin");
  if(header && jumpPoint == labelHash.end()) {
    std::cout << "Error: 'begin' label not found.\n";
    return false;
  }
  std::vector<char> newMachineCode;
  if(header) {
    int jump = jumpPoint->second + addrOff;
    createHeader(machineCode, newMachineCode, jump);  //create header
    writeMachineCodeToFile(newMachineCode, outfile);
  }
  else {
    writeMachineCodeToFile(machineCode, outfile);  //no header, so createHeader doesn't need to do anything
  }
  //for debugging, print the contents of machineCode
  //for(auto x : machineCode) { std::cout << HEX( x ) << '\n';}

  std::cout << "Assembly successful\n";
  return true;  //nothing went wrong
}
