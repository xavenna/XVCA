#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "file-utils.h"

namespace fs = std::filesystem;

bool isValidDriveName(std::string name) {
  try {
    if(name.substr(name.size()-4) == ".xdr") {
      return true;
    }
    return false;
  }
  catch (...) {
    return false;
  }
}

bool fileExists(std::string foldername) {
  return fs::exists(fs::current_path() / foldername);
}

void loadContentsOfFileToString(std::string fileName, std::string& buffer) {
  std::ifstream get(fileName);
  std::string tmp;
  if(!get.is_open()) {
    throw -1;
  }
  while(get.peek() != EOF) {
    std::getline(get, tmp);
    buffer += tmp + '\n';
  }
}
