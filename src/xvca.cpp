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


  bool helpMode = false;
  std::string targetDrive;
  for(int i=0;i<argc;i++) {
    
    std::string arg(argv[i]);  //this makes it easier to work with, and also I don't really like cstrings
    if(arg == "-h") {
      helpMode = true;
    }
    else if(arg.substr(0,2) == "-d") {
      if(arg.size() < 3) {
	std::cout << "Error: invalid usage of '-d' flag\n";
	return 1;
      }
      targetDrive = arg.substr(2);
      if(!isValidDriveName(targetDrive)) {
	std::cout << "Error: '" << targetDrive << "' is not a valid drive name.\n";
	return 1;
      }
    }
    else {
      std::cout << "Error: invalid argument.\n";
      return 1;
    }
  }

  if(helpMode) {
    std::cout << "XVCA VM: initial version.\n";
    std::cout << "usage: " << argv[0] << " [-h] -d<drivename>\n";
  }
  return beginEmulation(targetDrive);
}
