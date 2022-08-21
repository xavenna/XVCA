#ifndef DRIVE_ADAPTER_H
#define DRIVE_ADAPTER_H
#include <string>
#include "memory-group.h"

class DriveAdapter {
public:
  DriveCommandBuffer drcBuf;
  DriveBuffer driveBuf;
  std::string driveName;
  void loadSector(unsigned);
  void update();
  void updateStatus();
};


#endif
