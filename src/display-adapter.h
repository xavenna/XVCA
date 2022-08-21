#ifndef DISPLAY_ADAPTER_H
#define DISPLAY_ADAPTER_H
#include "memory-group.h"
#include <string>
#include <iostream>

class DisplayAdapter {
public:
  DisplayBuffer displayBuf;
  
  void updateDisplay();
  DisplayAdapter();
};

std::string toANSI(char, char);

#endif
