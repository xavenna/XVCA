#include "util.h"

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
