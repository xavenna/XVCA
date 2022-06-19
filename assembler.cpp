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
    //parse line, remove comments, transform into machine code
  }
  return true;  //so g++ doesn't get mad at me, remove once the function is finished
}
