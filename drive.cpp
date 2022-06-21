#include <fstream>
#include <filesystem>
#include <iostream>
#include <cmath>
#include "drive.h"
#include "util.h"
#include "file-utils.h"

namespace fs = std::filesystem;

bool packFolderToDrive(std::string folderName, std::string driveName) {
  if(fileExists(driveName) && !confirm("This file already exists. Are you sure you want to overwrite it?")) {
    return false;
  }
  std::ifstream get;
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
  for (const auto & entry : fs::directory_iterator(fs::current_path() / folderName)) {
    temp = entry.path().filename().string();
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

    get.open(temp, std::ios::binary);
    std::filebuf* pbuf = get.rdbuf();
    char* buffer=new char[size];
    pbuf->sgetn(buffer, size);
    get.close();

    //pad buffer 
    std::string fileContent(buffer);
    
    driveBody += padStringToSize(fileContent, sectors * 1024);
    delete[] buffer;
  }
  //write filetable to file
  fileTable = padStringToSize(fileTable, 1024); //pad filetable (eventually add size check)
  drive.write(fileTable.c_str(), fileTable.size());
  drive.write(driveBody.c_str(), driveBody.size());


  //now pad 

  return true;
}
