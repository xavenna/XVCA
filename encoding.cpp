#include <vector>
#include <string>
#include "encoding.h"


char chASCIItoXSCE(char in) {  //this seems to be broken
  std::vector<char> index{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   
    0x5e, 0x50, 0x2c, 0x59, 0x5a, 0x4e, 0x53, 0x2c, 0x0a, 0x0b, 0x4c, 0x4a, 0x57, 0x4b, 0x56, 0x4d,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x2e, 0x2f, 0x2a, 0x4f, 0x2b, 0x55,
    0x58, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
    0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x0c, 0x54, 0x0d, 0x51, 0x5b,
    0x5c, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
    0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x0e, 0x52, 0x0f, 0x5d, 0x64
  };
  return index[+in];   //I have no idea if this will work
}

char chXSCEtoASCII(char in) {
  std::vector<char> index{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '[', ']', '{', '}',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<', '>', '\'','"', ':', ';',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '+', '-', '*', '/', '%', '=',
    '!', '^', '|', '&', '\\','?', '.', ',', '@', '#', '$', '_', '`', '~', ' ', '\0',
    '\n','\r',8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
  };
  return index[+in];
}

std::string stASCIItoXSCE(std::string in) {
  for(auto& x : in) {
    x = chASCIItoXSCE(x);
  }
  return in;
}
std::string stXSCEtoASCII(std::string in) {
  for(auto& x : in) {
    x = chXSCEtoASCII(x);
  }
  return in;
}

void stASCIItoXSCEr(std::string& in) {
  for(auto& x : in) {
    x = chASCIItoXSCE(x);
  }
}
void stXSCEtoASCIIr(std::string& in) {
  for(auto& x : in) {
    x = chXSCEtoASCII(x);
  }
}

void blASCIItoXSCE(char* array, int length) {
  for(int i=0;i<length;i++) {
    array[i] = chASCIItoXSCE(array[i]);
  }
}

void blXSCEtoASCII(char* array, int length) {
  for(int i=0;i<length;i++) {
    array[i] = chXSCEtoASCII(array[i]);
  }
}
