#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <string>

//void sSplit(std::string, std::vector<std::string>&, std::string);
bool isLetter(char);
bool isNum(char);
bool confirm(std::string);
std::string padTo2Bytes(int);
//void padStringToSize(std::string&, size_t);
std::string padStringToSize(std::string, size_t);

#endif
