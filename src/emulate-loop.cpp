#include <iostream>
#include <string>
#include <fstream>

#include <curses.h>


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

  initscr();  //set up pdcurses window

  while(true) {
    if(!emulator.runCycle())
      break;
  }

  endwin();  //cleanup pdcurses
  
  return 0;  //assuming everything goes well
}
