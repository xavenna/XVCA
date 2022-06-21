#include <fstream>
#include <filesystem>
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include "drive.h"
#include "util.h"
#include "file-utils.h"

namespace fs = std::filesystem;

bool packFolderToDrive(std::string folderName, std::string driveName) {
  if(fileExists(driveName) && !confirm("This file already exists. Are you sure you want to overwrite it?")) {
    return false;
  }
  std::ofstream drive(driveName, std::ios::binary | std::ios::trunc);
  if(!drive.is_open()) {
    std::cout << "Error: file couldn't be opened.\n";
    return false;
  }

  //drive and folder exist, so begin constructing drive
  char blankSector[1024] = "\0";
  drive.write(blankSector, 1024);

  //create a file table
  std::string fileTable;
  std::string driveBody;
  
  int currentSector = 2;
  std::string temp;
  std::string ftemp;
  for (const auto & entry : fs::directory_iterator(fs::current_path() / folderName)) {
    temp = entry.path().filename().string();
    ftemp = entry.path().string();
    //temp is the file to be packed.
    //determine file size
    long size = fs::file_size(entry.path());
    int sectors = ceil((float) size / 1024);
    fileTable += padStringToSize(temp.substr(0, temp.find(".")).substr(0,8) + "." + temp.substr(temp.find(".")+1).substr(0,3), 12);
    //add file name to table
    fileTable += padTo2Bytes(currentSector);
    //determine next sector
    currentSector += sectors;
    fileTable += padTo2Bytes(currentSector-1); //this won't work for empty files, make sure this doesn't break things

    std::string buffer;
    loadContentsOfFileToString(ftemp, buffer);

    //pad buffer 
    
    driveBody += padStringToSize(buffer, sectors * 1024);
  }
  //write filetable to file
  fileTable = padStringToSize(fileTable, 1024); //pad filetable (eventually add size check)
  drive.write(fileTable.c_str(), fileTable.size());
  drive.write(driveBody.c_str(), driveBody.size());


  //now pad 

  return true;
}

bool addFileToBootSector(std::string driveName, std::string bootFileName) {
  //check if boot sector is empty
  std::fstream driveAccess;
  std::ifstream fileAccess(bootFileName, std::ios::binary);
  char buffer[1024];
  std::string emptySector(1024, '\0');
  std::string newDrive;
  driveAccess.open(driveName, std::ios::in | std::ios::binary);
  driveAccess.read(buffer, 1024);

  if(emptySector != buffer && !confirm("The boot sector is not empty. Are you sure you want to overwrite it?")) {
    return false;
  }

  //write first 1 KiB of bootFileName to newDrive
  fileAccess.read(buffer, 1024);
  newDrive += padStringToSize(buffer, 1024);

  //now get the rest of the file into newDrive
  size_t fileSize = fs::file_size(fs::current_path() / driveName);
  std::cout << "size of '" << (fs::current_path() / driveName).string() << "' is " << fileSize << '\n';
  size_t remainingSize = fileSize - 1024;
  char* newBuffer = new char[remainingSize];
  driveAccess.read(newBuffer, remainingSize);
  std::cout << remainingSize << '\n' << newBuffer << '\n';
  driveAccess.close();
  driveAccess.open(driveName, std::ios::out | std::ios::trunc | std::ios::binary);
  driveAccess.write(newDrive.data(), newDrive.size());
  driveAccess.write(newBuffer, remainingSize);  //this has to not pass through a std::string because it contains null characters and the string copy doesn't like that
  return true;
} 


bool listDriveFiles(std::string driveName) {  //this is very c library heavy due to mostly using cstrings
  std::ifstream driveAccess(driveName, std::ios::binary);
  driveAccess.seekg(1024);  //beginning of file table in sector 1
  char buffer[1024];
  driveAccess.read(buffer, 1024);
  //buffer now contains the file table. Now, parse it
  //each entry is 16 bytes long
  //so there are 64 potential entries (unless the format changes)
  for(int i=0;i<64;i++) {
    char entry[16];
    memcpy(entry, buffer+(16*i), 16); //string isn't necessarily null-terminated, so be careful
    if(entry[0] == '\0')
      break; //table is over
    fwrite(entry, sizeof(char), 12, stdout);
    std::cout << ":\tFrom " << +entry[12] << +entry[13] << " to " << +entry[14] << +entry[15] << '\n';
    //this string formatting will break if sector numbers go above 9, write a function to turn a char into a decimal number
  }
  return true;
}



