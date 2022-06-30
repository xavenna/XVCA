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
      std::cout << "XVCA Assembler: version Test0.0\n";
      std::cout << "Usage: " << argv[0] << " <input file> <output file>\n";
    }
    else {
      //error
    }
  }
  else if(argc == 3) {
      //attempt to assemble second argument into third argument
    if(!assemble(argv[1], argv[2])) {
      std::cout << "Error: specified file could not be assembled.\n";
      //error
    }
    else {
      //success
    }
  }
  else {
    std::cout << "Error: invalid command. Use '" << argv[0] << " -h' to see help.\n";
  }
}

bool assemble(std::string filename, std::string outfile) {
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
      return false;
    }
    codesPerLine.push_back(value);
  }
  fixLabelJumpPoints(machineCode, labelHash, jumpHash, codesPerLine);  //jumpHash: {address location in bytecode, target label}
  auto jumpPoint = labelHash.find("begin");
  if(jumpPoint == labelHash.end()) {
    std::cout << "Error: 'begin' label not found.\n";
    return false;
  }
  int jump = jumpPoint->second + 0x1000;  //0x1000 is the offset
  //std::cout << "jump point rel: " << jumpPoint->second << " abs: " << jump << '\n';
  std::vector<char> newMachineCode;
  createHeader(machineCode, newMachineCode, jump);  //create header
  //for debugging, print the contents of machineCode
  //for(auto x : machineCode) { std::cout << HEX( x ) << '\n';}
  //write assembly to file
  writeMachineCodeToFile(newMachineCode, outfile);
  return true;  //nothing went wrong
}
