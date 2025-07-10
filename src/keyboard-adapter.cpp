#include "keyboard-adapter.h"
#include <unistd.h>
#include <iostream>


int KeyboardAdapter::updateBuffer() {
  char c = 0;
  std::string buf;
  do {
    read(STDIN_FILENO, &c, 1);
    if(c == 0x03) {
      //C-c: shutdown
      return 1;
    }
    else if(c == 0x04) {
      //C-d: trigger a forced interrupt.
      return 4;
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
  if(!strncmp(buf.c_str(),keyboardBuffer.buffer.begin(), keyboardBuffer.buffer.size())) {
    return 0;
  }
  memcpy(keyboardBuffer.buffer.data(), buf.c_str(), 8);
  std::copy(buf.begin(), buf.end(), keyboardBuffer.buffer.begin());
  return 3;
}
