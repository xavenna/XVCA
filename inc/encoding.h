#ifndef ENCODING_H
#define ENCODING_H
#include <vector>
#include <string>

char chASCIItoXSCE(char);
char chXSCEtoASCII(char);
void stASCIItoXSCEr(std::string&);
void stXSCEtoASCIIr(std::string&);
std::string stASCIItoXSCE(std::string);
std::string stXSCEtoASCII(std::string);
void blASCIItoXSCE(char*, int);
void blXSCEtoASCII(char*, int);

#endif
