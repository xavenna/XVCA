#include <string>
#include <iostream>

#include "display-adapter.h"


void DisplayAdapter::updateDisplay() {
  //convert buffer from XSCE to ASCII, but not yet. conversion is broken, so just use ASCII

  std::string buffer;
  for(int i=0;i<25;i++) {
    //for each display row
    for(int j=0;j<40;j++) {
      //for each pair of bytes

      //convert and echoas a character
      buffer += toANSI(displayBuf.buffer[i*80 + j*2], displayBuf.buffer[i*80 + j*2 + 1]);
    }
    if(i != 24) {
      buffer += "\x1b[1B";
    }
    else {
      buffer += "\x1b[H";
    }
      
  }
  std::cout << buffer;
}

DisplayAdapter::DisplayAdapter() {
  
}

std::string toANSI(char fc, char sc) {
  std::string result = "\x1b[3";
  char fgcc = '0' + ((sc & 0xe0) >> 5);
  char bgcc = '0' + ((sc & 0x0e) >> 1);
  result += fgcc;
  result += ";4";
  result += bgcc;
  result += "m";
  result += fc;
  result += "\x1b[0m";
  return result;
}
