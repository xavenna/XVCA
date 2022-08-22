#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <termios.h>


#include "emulator.h"
#include "drive.h"

struct termios orig_termios;
void initializeTerm();
void resetTerm();

int beginEmulation(std::string targetDriveName) {
  //targetDriveName is a valid xvca drive name

  if(!verifyDrive(targetDriveName)) {
    std::cout << "Error: Invalid drive.\n";
    return 1;
  }


  AdapterGroup ag; //I have to construct Emulator in this way because if I don't, it segfaults or excepts and I don't know why. Also, I don't know why this fixes it.
  Emulator emulator(ag);
  emulator.setDrive(targetDriveName);

  //write initial bootloader to memory
  std::string bootload;
  std::ifstream getBoot("boot.x");
  while(getBoot.peek() != EOF) {
    bootload += getBoot.get();
  }

  emulator.cpu.memory.writeBlock(0, bootload.c_str(), bootload.size());
  emulator.cpu.registers.stackPointer = 0xf000;
  

  initializeTerm();

  while(true) {
    if(!emulator.runCycle())
      break;
    //add debugging thingies
  }


  return 0;  //assuming everything goes well
}

void initializeTerm() {
  std::cout << "\x1b[2J\x1b[=1h\x1b[H";  //prepare screen

  //enable raw mode
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(resetTerm);

  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void resetTerm() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
