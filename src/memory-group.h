#ifndef MEMORY_GROUP_H
#define MEMORY_GROUP_H
#include <cstdint>

class DriveBuffer {
public:
  char buffer[1024];  
  //I don't think this really needs to do much, the driveAdapter does most of the hard work
};

class DisplayBuffer {
public:
  char buffer[2000];
  //same as above
};

class DriveCommandBuffer {
public:
  char buffer[8];  //only the first three bytes are used, but I wanted room for further expansion
};

class KeyboardBuffer {
public:
  char buffer[8];  //only the first byte is currently used
};

class ShutdownBuffer {
public:
  char buffer;     //only $FFFF is used, so there was no point using more
};


class MemoryGroup {
  char primaryMemory[62464];
  DriveBuffer& driveBuf;
  DisplayBuffer& dispBuf;
  KeyboardBuffer& keyBuf;
  DriveCommandBuffer& drcBuf;
  ShutdownBuffer& shutdownBuf;
public:
  void write(uint16_t, char);
  char read(uint16_t);
  MemoryGroup(DriveBuffer, DisplayBuffer, KeyboardBuffer, DriveCommandBuffer, ShutdownBuffer);
};

#endif
