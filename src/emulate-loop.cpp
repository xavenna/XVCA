#include <iostream>
#include <string>
#include <fstream>


#include "emulator.h"
#include "drive.h"


int beginEmulation(std::string targetDriveName) {
  //targetDriveName is a valid xvca drive name

  if(!verifyDrive(targetDriveName)) {
    std::cout << "Error: Invalid drive.\n";
    return 1;
  }

  Emulator emulator;
  emulator.setDrive(targetDriveName);

  std::cout << "\x1b[=1h";

  while(true) {
    if(!emulator.runCycle())
      break;
  }

  return 0;  //assuming everything goes well
}
