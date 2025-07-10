#ifndef MEMORY_GROUP_H
#define MEMORY_GROUP_H
#include <cstdint>
#include <array>

class DriveBuffer {
public:
  std::array<char, 1024> buffer;  
};

class DisplayBuffer {
public:
  std::array<char, 2000> buffer;
  //same as above
};

class DriveCommandBuffer {
public:
  std::array<char, 8> buffer;  //only the first three bytes are used, but I wanted room for further expansion
};

class KeyboardBuffer {
public:
  std::array<char, 8> buffer;  //only the first byte is currently used
};

class ShutdownBuffer {
public:
  char buffer;     //only $FFFF is used, so there was no point using more
};


class MemoryGroup {
  std::array<char, 62464> primaryMemory;
  DriveBuffer& driveBuf;
  DisplayBuffer& dispBuf;
  KeyboardBuffer& keyBuf;
  DriveCommandBuffer& drcBuf;
  ShutdownBuffer& shutdownBuf;
public:
  void write(uint16_t, const char);
  bool writeBlock(uint16_t, const char*, unsigned);
  char read(uint16_t) const;
  bool readBlock(uint16_t, char*, unsigned) const;
  MemoryGroup(DriveBuffer&, DisplayBuffer&, KeyboardBuffer&, DriveCommandBuffer&, ShutdownBuffer&);
};

#endif
