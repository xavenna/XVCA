#include "memory-group.h"
#include <iostream>

void MemoryGroup::write(uint16_t address, const uint8_t data) {
  //std::cerr << "Writing 0x"<<std::hex<<+(data & 0xff)<<" to address 0x"<<address<<std::dec<<"\n";
  if(address < 0xf400) {
    primaryMemory[address] = data;
  }
  else if(address >= 0xf400 && address < 0xf800) {
    //drive buffer
    //read-only
  }
  else if(address >= 0xf800 && address < 0xffd0) {
    //display buffer
    dispBuf.buffer[address-0xf800] = data;
  }
  else if(address >= 0xffd0 && address < 0xffd8) {
    //keyboard buffer
    //read-only
  }
  else if(address >= 0xffd8 && address < 0xffe0) {
    //drive command buffer
    drcBuf.buffer[address-0xffd8] = data;
  }
  else if(address == 0xffff) {
    //shutdown buffer
    shutdownBuf.buffer = data;
  }
  else {
    //does nothing, as it is unmapped MMIO
  }
}
uint8_t MemoryGroup::read(uint16_t address) const {
  //std::cerr << "Reading from address 0x"<<std::hex<<address<<std::dec<<"\n";
  char temp;
  if(address < 0xf400) {
    temp = primaryMemory[address];
  }
  else if(address >= 0xf400 && address < 0xf800) {
    //drive buffer
    temp = driveBuf.buffer[address-0xf400];
  }
  else if(address >= 0xf800 && address < 0xffd0) {
    //display buffer
    temp = dispBuf.buffer[address-0xf800];
  }
  else if(address >= 0xffd0 && address < 0xffd8) {
    //keyboard buffer
    temp = keyBuf.buffer[address-0xffd0];
  }
  else if(address >= 0xffd8 && address < 0xffe0) {
    //drive command buffer
    //write-only
    //actually, no, arguments...
    temp = keyBuf.buffer[address-0xffd8];
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

bool MemoryGroup::readBlock(uint16_t addr, char* buffer, unsigned len) const {
  for(unsigned i=0;i<len;i++) {
    buffer[i] = read(addr+i);
  }
  return true; //can return false if fail
}

bool MemoryGroup::writeBlock(uint16_t addr, const char* buffer, unsigned len) {
  for(unsigned i=0;i<len;i++) {
    write(addr+i, static_cast<uint8_t>(buffer[i]));
  }
  return true; //can return false if fail
}

MemoryGroup::MemoryGroup(DriveBuffer& db, DisplayBuffer& dsb, KeyboardBuffer& kb, DriveCommandBuffer& dbc, ShutdownBuffer& sb) : driveBuf{db}, dispBuf{dsb}, keyBuf{kb}, drcBuf{dbc}, shutdownBuf{sb} {

}
