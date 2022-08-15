#ifndef ADAPTER_GROUP_H
#define ADAPTER_GROUP_H
#include "display-adapter.h"
#include "drive-adapter.h"
#include "keyboard-adapter.h"

#include "memory-group.h"


class AdapterGroup {  //add other adapters eventually
public:
  DisplayAdapter displayAdapter;
  DriveAdapter driveAdapter;
  KeyboardAdapter keyboardAdapter;
  ShutdownBuffer shutdownBuf;
};

#endif
