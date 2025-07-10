#include <iostream>
#include <string>
#include <curses.h>

#include "emulator.h"
#include "file-utils.h"
#include "emulate-loop.h"
#include "disassemble.h"
/*  XVCA: a virtual computer
 *  Components:
 *  Assembler: transforms assembly into bytecode
 *  Computer:  the main component; actually performs the simulation
 *  Disassembler: generates assembly from bytecode
 *  Drive Editor:  manages XVCA drive files

 */

int main(int argc, char** argv) {

  if(argc == 1) {
    std::cout << "Error: no arguments supplied. Use '" << argv[0]
	      << " -h' for usage information.\n";
  }
  else if(argc == 2) {
    std::string arg(argv[1]);
    if(arg == "-h") {
      //help
      std::cout << "XVCA VM: initial version.\n";
      std::cout << "usage: " << argv[0] << " [-h] -d<drivename>\n";
    }
    else {
      if(!isValidDriveName(arg)) {
        std::cout << "Error: '" << arg << "' is not a valid drive name.\n";
        return 1;
      }
      //attempt to run the thingy as a drive
      return beginEmulation(arg);
    }
  }
  else if(argc == 3) {
    //expects flag then executable
    std::string arg1(argv[1]);
    std::string arg2(argv[2]);
    if(arg1 == "-h") {
      std::cout << "XVCA VM: initial version.\n";
      std::cout << "usage: " << argv[0] << " [-h] -d<drivename>\n";
    }
    else if(arg1 == "-d") {
      //generate a disassembly for the provided binary
      return disassemble(arg2);
    }
    else {
      std::cerr << "Error: Expects "<<argv[0]<<" [flag] <boot drive>\n";
    }


  }
}
