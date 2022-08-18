#include "keyboard-adapter.h"


void KeyboardAdapter::updateBuffer() {
  int c;
  std::string buf;
  do {
    c = getchar();
    if(c != EOF) {
      buf += c;
    }
    else {
      c = 0;
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
  }while(true);
  //pad string with null characters, if necessary
  std::string pad(0x0, buf.size() < 8 ? 8-buf.size() : 1);
  buf += pad;
  memcpy(keyboardBuffer.buffer, buf.c_str(), 8);
}
