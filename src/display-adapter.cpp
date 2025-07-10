#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>

#include "display-adapter.h"

void DisplayAdapter::updateDisplay() {
  //this is going to be slow
  //this is a partial version of the display adapter created to test the system
  const int lineSize{40};
  const int lineNum{25};
  std::string buffer;
  for(int l=0;l<lineNum;l++) {
    buffer.clear();
    for(int i=0;i<lineSize;i++) {
      //take two chars
      char fc = displayBuf.buffer[l*lineSize*2 + 2*i];
      char sc = displayBuf.buffer[l*lineSize*2 + 2*i+1];  //this is ignored for now
      
      //this is both for debugging and so gcc doesn't complain 
      
      //char newchar = (fc > 0x1f && fc < 0x7f) ? fc : 0x20;
      buffer += toANSI(fc, sc);
      //buffer += fc;
    }
    fputs(buffer.c_str(), stdout);
    putc('|', stdout);
    if(l+1 != lineNum) {
      fputs("\x1b[0G\x1b[1B", stdout);
    }
    else {
      fputs("\x1b[H", stdout);
    }
  }
}

void DisplayAdapter::updateDisplayFull() {
  //This is broken and needs to be rewritten
  //for now, ignore color and simply make it write to the screen
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
      buffer += "|\n";
    }
    else {
    }
      
  }
  std::cout << "\x1b[H";
  puts(buffer.c_str());

}

DisplayAdapter::DisplayAdapter() {
  
}

std::string toANSI(char fc, char sc) {
  std::string result;
  if(fc > 31 && fc < 127) {
    result = fc;
  }
  else {
    result = " ";
  }
  return result;
  //std::string result = "\x1b[3";
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
