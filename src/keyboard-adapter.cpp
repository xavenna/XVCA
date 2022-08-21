#include "keyboard-adapter.h"
#include <unistd.h>
#include <iostream>


bool KeyboardAdapter::updateBuffer() {
  //THIS DOESN"T WORK, REWRITE
  //std::cout << "updating buffer\n";
  char c = 0;
  std::string buf;
  do {
    read(STDIN_FILENO, &c, 1);
#ifdef XV_DEBUG
    //std::cout << +c << "\n";
#endif
    if(c == 0x03) {
      //C-c: interrupt, shutdown
      return false;
    }
    if(c != 0x0) {
      buf += c;
    }
    else { //no input
      c = 0;
      //clear buffer
      buf = "\0\0\0\0\0\0\0\0\0";
      break;
    }
    if(buf[0] != 0x1b) {
      //character isn't an escape sequence, done
      break;
    }
    else {
      if((buf.size() == 3 && (buf == "\x1b[A" || buf == "\x1b[B" || buf == "\x1b[C" || buf == "\x1b[D")) || (buf.size() == 4 && buf == "\x1b[3~")) {
	//complete escape sequence
	break;
      }
      else if(buf.size() == 8) {
	//unrecognized escape, break and empty string
      }
    }
    //std::cout << "end of loop\n";
  }while(true);
  //pad string with null characters, if necessary
  for(int i=0;i<8;i++) {
    buf += '\0';
  }
  memcpy(keyboardBuffer.buffer, buf.c_str(), 8);
  return true;
}
