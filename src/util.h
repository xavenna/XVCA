#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>

void split(const std::string& n, std::vector<std::string>& th);
bool isLetter(char);
bool isNum(char);
bool isNum(const std::string& st);
bool isNum(const std::string st, unsigned radix);
bool isEmpty(char c);
bool confirm(std::string);
std::string padTo2Bytes(int);
//void padStringToSize(std::string&, size_t);
std::string padStringToSize(std::string, size_t);
void parse(const std::string& n, std::vector<std::string>& th, std::string key);
bool ifMatch(char n, std::string key);

#endif
