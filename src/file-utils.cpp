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
  get.close();
  if(isBinFile(fileName)) {
    //remove final newline
    buffer.pop_back();
  }
}

bool isPrintable(char s) {
  return ((s >= 0x20 && s < 0x80) || s == '\n' || s == '\t' || s == '\r');
}
bool isBinFile(std::string fname) {
  std::ifstream get(fname);
  if(!get.is_open()) {
    throw -1;
  }
  while(get.peek() != EOF) {
    if(!isPrintable(get.get())) {
      return true;
    }
  }
  return false;
}

size_t binFileSize(std::ifstream& in) {
	if(!in.is_open())
		return false;
	in.ignore( std::numeric_limits<std::streamsize>::max() );
	std::streamsize length = in.gcount();
	in.clear();   //  Since ignore will have set eof.
	in.seekg( 0, std::ios_base::beg );
	return length;
}

