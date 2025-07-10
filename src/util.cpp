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
bool isNum(const std::string& st) {
  for(unsigned i=0;i<st.size();i++) {
    if(!isdigit(st[i]) && (st[i] != '-' || i != 0))
      return false;
  }
  return true;
}
bool isNum(const std::string st, unsigned radix) {
  if(radix > 36) {
    return false;
  }
  std::string validChars;
  validChars.reserve((radix <= 10) ? radix : (radix-10)*2 + 10);
  for(unsigned i=0;i<radix;i++) {
    if(i < 10) {
      validChars += ('0'+i);
    }
    else {
      validChars += ('a'+i-10);
      validChars += ('A'+i-10);
    }
  }
  for(unsigned i=0;i<st.size();i++) {
    if(!(ifMatch(st[i], validChars)) && (st[i] != '-' || i != 0))
      return false;
  }
  return true;
}

  
bool isEmpty(char c) {
  return (c == ' ' || c == '\t'); 
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
//this overload was removed because it is indistinguishable from the other one and g++ doesn't like that :(

std::string padStringToSize(std::string string, size_t size) {
  std::string pad(size-string.size(), '\0');
  return string += pad;
}
bool ifMatch(char n, std::string key) {
	return (key.find(n) != std::string::npos);
}

void parse(const std::string& n, std::vector<std::string>& th, std::string key) {
  //each entry is a string delimited by a character in key, similar to bk but more general
  std::string cw;
  for(size_t i=0;i<n.size();i++) {
    if(ifMatch(n[i], key)) {
      if(cw.empty())
	cw += '\0';
      th.push_back(cw);
      cw.clear();
    }
    else {
      cw += n[i];
    }
  }
  th.push_back(cw);
}
