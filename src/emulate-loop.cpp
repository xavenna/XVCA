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
  const char* bootload = "\x06\x01\x15\xFF\xD8\x0B\xFF\xD9\x41\x10\x15\x52\x01\x41\x10\x53\x58\x02\x41\x10\x67\x06\x00\x15\xFF\xDA\x06\x10\x15\xFF\xD8\x3D\x09\x13\x0A\xFF\x06\xE4\x50\x03\x04\x00\x10\x02\x00\x01\x03\x58\xE4\x04\x00\x01\x01\x10\x01\x04\x58\x01\x05\x00\x01\x03\x59\x00\x04\x00\x06\xFF\x5C\x04\x44\x10\x24\x06\x0F\x5C\x03\x44\x10\x24\x40\x10\x00\x06\x14\x15\xF8\x00\x06\x01\x15\xF8\x01\x06\x34\x15\xF8\x0F\x06\xFF\x15\xFF\xFF";

  emulator.cpu.memory.writeBlock(0, bootload, 0x66);
  

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
