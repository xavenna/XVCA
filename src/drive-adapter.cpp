#include <filesystem>
#include <fstream>
#include "drive-adapter.h"

namespace fs = std::filesystem;

void DriveAdapter::update() {
  //check for commands
  switch(drcBuf.buffer[0]) {
  case 0x01:
    //query status
    updateStatus();
    break;
  case 0x10:
    //load sector from drive to buffer
    loadSector(drcBuf.buffer[2]);
    break;
  case 0x20:
    //write sector to drive
    throw std::invalid_argument("DriveAdapter::update(): not implemented");
    break;
  default:
    //do nothing
    break;
  }

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

  if(!fsize % 1024 == 0) {
    status = 0x01;
  }

  //write status
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

  if(fsize < (sect-1)*1024) {
    throw std::invalid_argument("DriveAdapter::loadSector(): Invalid sector");
  }
  //seek driveName
  load.seekg((sect-1)*1024);

  //now load the sector into the thingy
  load.read(driveBuf.buffer, 1024);

  load.close();
}
