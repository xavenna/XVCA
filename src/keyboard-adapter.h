#ifndef KEYBOARD_ADAPTER_H
#define KEYBOARD_ADAPTER_H
#include <stdio.h>
#include <string>
#include <cstring>

#include "memory-group.h"

class KeyboardAdapter {
public:
  KeyboardBuffer keyboardBuffer;
  void updateBuffer();
};


#endif
