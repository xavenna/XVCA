#include <filesystem>
#include <fstream>
#include "drive-adapter.h"
#include <iostream>

namespace fs = std::filesystem;

void DriveAdapter::update() {
  //check for commands
  switch(drcBuf.buffer[0]) {
  case 0x01:
    //query status
    //std::cerr << "Drive status queried\n";
    updateStatus();
    break;
  case 0x10:
    //load sector from drive to buffer
    //std::cerr << "Loading sector to buffer\n";
    loadSector(drcBuf.buffer[2]);
    break;
  case 0x20:
    //write sector to drive
    //std::cerr << "Attempt to write to drive failed - not implemented\n";
    break;
  default:
    //do nothing
    break;
  }
  //reset buffer
  drcBuf.buffer[0] = 0;

  //if necessary, update drive buffer
}

void DriveAdapter::updateStatus() {
  fs::path drive(driveName);
  uint8_t status = 0;
  //check if drive is inserted
  std::ifstream n(driveName);
  if(!n.is_open()) {
    status = 0xff;
  }


  size_t fsize = fs::file_size(drive);

  if(!(fsize % 1024 == 0)) {
    std::cerr << "Error: extra "<<(fsize%1024)<<" bytes\n";
    status = 0x01;
  }

  //write status
  //std::cerr << "Wrote '"<<+status<<"' to status\n";
  drcBuf.buffer[1] = status;
}

void DriveAdapter::loadSector(unsigned sect) {
  fs::path drive(driveName);
  std::ifstream load(driveName, std::ios::binary);

  if(!load.is_open()) {
    throw std::invalid_argument("DriveAdapter::loadSector(): Invalid drive name");
  }

  //check drive length
  size_t fsize = fs::file_size(drive);
  //fsize is now size of the file

  if(fsize < (sect)*0x400) {
    std::string msg = "DriveAdapter::loadSector(): Invalid sector ";
    msg += std::to_string(sect) + ".";
    throw std::invalid_argument(msg);
  }
  //seek driveName
  load.seekg((sect)*0x400);

  //now load the sector into the thingy
  load.read(driveBuf.buffer.begin(), 0x400);

  load.close();
}
