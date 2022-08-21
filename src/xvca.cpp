#include <iostream>
#include <string>
#include <curses.h>

#include "emulator.h"
#include "file-utils.h"
#include "emulate-loop.h"
/*  XVCA: a virtual computer
 *  Components:
 *  Assembler: transforms assembly into bytecode
 *  Computer:  the main component; actually performs the simulation
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
}
