#ifndef EMULATOR_H
#define EMULATOR_H
#include <cstdint>
#include <string>
//maybe split classes into different headers
#include "register-group.h"
#include "memory-group.h"
#include "adapter-group.h"
#include "cpu.h"




class Emulator {
public:
  AdapterGroup& adapterGroup;
  CPU cpu;
  std::string driveName;
  
  void setDrive(std::string);
  int updateHardware();
  int runCycle();
  Emulator(AdapterGroup&);
private:
  long int startingTime;
  long int getTimeDiff(); //in us
  long int lastFrameEndTime; //in us
  const static long int DispUpdateDuration = 80000; // this means a 20 Hz display

  long long int preciseTime;
  long long int getPreciseTimeDiff();
  long long unsigned cycleNum=0; //!< what execution cycle the program is on
  long long unsigned lastHardwareUpdateCycleNum=0;
};
#endif
