#include "assembler.h"




int rewriteNum(std::string n) {
  if(n.substr(0,1) == "$") {
    //number is hex
    return std::stoi (n.substr(1),nullptr,16);
  }
  else {
    //number is decimal
    //std::cout << n << ',' << std::stoi(n) << '\n';
    return std::stoi (n);
  }
  throw -1;
}
