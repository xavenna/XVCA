#include <iostream>
#include <fstream>
#include "util.h"
#include "file-utils.h"
#include "drive.h"

/* This is the XVCA Drive manager
 *  It is used to pack files into XVCA Drives

 *  TODO: Add a function to transform encoding from ASCII to XSCE

 *  Functions:  (those marked with a * are complete, # means priority to complete)
 *  *-p : Packs files from a specified folder into a drive. 4 arguments
 *   -a : add. This will attempt to add a file to a drive. 4 arguments
 *  *-l : list. This lists all files contained in a drive. 3 arguments 
 *  *-h : help. This will display a help message. 2 arguments
 *  *-v : version. This will display version information. 2 arguments
 *  *-b : boot. This adds a specified file to the boot sector of drive. 4 arguments
 *   -u : unpack. This will attempt to extract all files from a drive. 4 arguments
 */

// Version 0.1.0-pre
int main(int argc, char** argv) {
  if(argc == 1) {
    std::cout << "Error: No arguments supplied. Use '" << argv[0] << " -h' for help\n";
    return 1;
  }
  else if(argc == 2) {
    std::string arg(argv[1]);
    if(arg == "-h") {
      //print help message
      std::cout << "XVCA Drive Manager v0.1.0-pre\n";
      std::cout << "Functions:\n";
      std::cout << "Pack:\tPacks all files from the specified folder into specified drive folder. Overwrites drive if it exists.\n\tUsage: " << argv[0] << " -p <folder> <drive>\n";
      std::cout << "List:\tLists all files contained in specified drive.\n\tUsage: " << argv[0] << " -l <drive>\n";
      std::cout << "Boot:\tAdds specified file to boot sector of specified drive\n\tUsage: " << argv[0] << " -p <drive> <file>\n";
      std::cout << "Help:\tDisplays this message.\n\tUsage: " << argv[0] << "-h\n";
      std::cout << "Version:Displays version information.\n\tUsage: " << argv[0] << "-v\n";
    }
    else if(arg == "-v") {
      std::cout << "XVCA Drive Manager: v0.1.0-pre\n";
    }
    else {
      std::cout << "Error: Unknown command.\n";
    }
  }
  else if(argc == 3) {
    std::string arg1(argv[1]);
    std::string arg2(argv[2]);
    if(arg1 == "-l") {
      //List files contained in a drive
      //Usage: drivemgr -l <drive>
      if(fileExists(arg2) && isValidDriveName(arg2)) {
	listDriveFiles(arg2);
      }
      else {
	std::cout << "Error: invald usage of the list command.\n";
      }
    }
    else {
      std::cout << "Error: invalid command.\n";
    }
  }
  else if(argc == 4) {
    std::string arg1(argv[1]);
    std::string arg2(argv[2]);
    std::string arg3(argv[3]);
    if(arg1 == "-p") {
      //pack files into a drive.
      //Usage: drivemgr -p <folder> <drive>
      if(fileExists(arg2) && isValidDriveName(arg3)) {
	packFolderToDrive(arg2, arg3);
      }
    }
    else if(arg1 == "-a") {
      //add file to existing drive.
      //Usage: drivemgr -a <drive> <file>
      std::cout << "Error: This functionality doesn't exist yet.\n";
    }
    else if(arg1 == "-b") {
      //add file to boot sector of existing drive.
      //Usage: drivemgr -b <drive> <file>
      if(fileExists(arg2) && fileExists(arg3) && isValidDriveName(arg2)) {
	addFileToBootSector(arg2, arg3);
      }
      //std::cout << "Error: This functionality doesn't exist yet.\n";
    }
    else {
      std::cout << "Error: invalid command.\n";
    }
  }
  
}
