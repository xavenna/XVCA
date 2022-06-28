#include "memory-group.h"

void MemoryGroup::write(uint16_t address, char data) {
  if(address >= 0 && address < 62464) {
    primaryMemory[address] = data;
  }
  else if(address >= 62464 && address < 63488) {
    //drive buffer
    //read-only
  }
  else if(address >= 63488 && address < 65488) {
    //display buffer
    dispBuf.buffer[address-63488] = data;
  }
  else if(address >= 65488 && address < 65496) {
    //keyboard buffer
    //read-only
  }
  else if(address >= 65496 && address < 65504) {
    //drive command buffer
    drcBuf.buffer[address-65496] = data;
  }
  else if(address == 65535) {
    //shutdown buffer
    shutdownBuf.buffer = data;
  }
  else {
    //does nothing, as it is unmapped MMIO
  }
}
char MemoryGroup::read(uint16_t address) {
  char temp;
  if(address >= 0 && address < 62464) {
    temp = primaryMemory[address];
  }
  else if(address >= 62464 && address < 63488) {
    //drive buffer
    temp = driveBuf.buffer[address-62464];
  }
  else if(address >= 63488 && address < 65488) {
    //display buffer
    temp = dispBuf.buffer[address-63488];
  }
  else if(address >= 65488 && address < 65496) {
    //keyboard buffer
    temp = keyBuf.buffer[address-65488];
  }
  else if(address >= 65496 && address < 65504) {
    //drive command buffer
    //write-only
  }
  else if(address == 65535) {
    //shutdown buffer
    //write-only
  }
  else {
    //does nothing, as it is unmapped MMIO
  }
  return temp;
}
MemoryGroup::MemoryGroup(DriveBuffer db, DisplayBuffer dsb, KeyboardBuffer kb, DriveCommandBuffer dbc, ShutdownBuffer sb) : driveBuf{db}, dispBuf{dsb}, keyBuf{kb}, drcBuf{dbc}, shutdownBuf{sb} {
  
}
