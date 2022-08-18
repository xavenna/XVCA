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

  Emulator emulator;
  emulator.setDrive(targetDriveName);

  initializeTerm();

  while(true) {
    if(!emulator.runCycle())
      break;
  }

  return 0;  //assuming everything goes well
}

void initializeTerm() {
  std::cout << "\x1b[=1h";
  //enable raw mode
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(resetTerm);

  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void resetTerm() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
