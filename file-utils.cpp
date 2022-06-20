#include <string>
#include <filesystem>
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
