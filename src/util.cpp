#include "util.h"
#include <iostream>
#include <string>

/*
void sSplit(std::string inputString, std::vector<std::string>& wordList, std::string key) { //splits a string into a deque
  //each entry is a string delimited by a character in key
  std::string currentWord;
  for(size_t i=0;i<inputString.size();i++) {
    if(ifMatch(inputString[i], key)) {
      if(currentWord.empty())
	currentWord += '\0';
      wordList.push_back(currentWord);
      currentWord.clear();
    }
    else {
      currentWord += inputString[i];
    }
  }
}  */

bool isLetter(char x) {
  return ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'));
}
bool isNum(char x) {
  return (x >= '0' && x <= '9');
}
bool confirm(std::string prompt) {
  std::string input;
  std::cout << prompt << "\n> ";
  std::cin >> input;
  if(input.size() == 0)
    return false;
  if(input[0] == 'y' || input[0] == 'Y')
    return true;
  return false;
}
std::string padTo2Bytes(int currentSector) {
  if(currentSector > 65535) {
    throw -1; //error: invalid sector
  }
  std::string output;
  output += (char) ((currentSector & 0xff00) >> 8);
  output += (char) (currentSector & 0xff);
  return output;
}

/*void padStringToSize(std::string& string, size_t size) {
  std::string pad(size-string.size(), '\0');
  string += pad;
  return;
  }*/
//this overload was removed because it is indistinguishable from the other one and g++ doesn't like that

std::string padStringToSize(std::string string, size_t size) {
  std::string pad(size-string.size(), '\0');
  return string += pad;
}
