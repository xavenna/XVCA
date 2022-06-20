#include <iostream>
#include <fstream>
#include "util.h"
#include "file-utils.h"
#include "drive.h"

/* This is the XVCA Drive manager
 *  Eventually, this will be able to pack data into a xvca drive

 *  Functions:
 *   -p : pack. This will pack files from a specified folder into a drive. 4 arguments
 *   -a : add. This will attempt to add a file to a drive. 4 arguments
 *   -l : list. This will list all files contained in a drive. 3 arguments 
 *   -h : help. This will display a help message. 2 arguments
 *   -v : version. This will display version information. 2 arguments
 *   -b : boot. This will add a specified file to the boot sector of drive. 4 arguments
 */
int main(int argc, char** argv) {
  if(argc == 1) {
    std::cout << "Error: No arguments supplied. Use '" << argv[0] << " -h' for help\n";
    return 1;
  }
  else if(argc == 2) {
    std::string arg(argv[1]);
    if(arg == "-h") {
      //print help message
      std::cout << "Useful Help Message\n";
    }
    else {
      std::cout << "Error: '" << arg << "' is not a valid XVCA drive filename. Files should have the extension .xdr\n";
    }
  }
  else if(argc == 3) {
    std::string arg1(argv[1]);
    std::string arg2(argv[2]);
    if(arg1 == "-l") {
      
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
  }
  
}
