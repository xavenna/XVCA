#ifndef ASSEMBLER_UTIL_H
#define ASSEMBLER_UTIL_H
#include <vector>
#include <string>
#include <map>

struct forEntry {
  unsigned line;
  char reg;
  int increment;
  int endval;
};
int rewriteNum(std::string);
int transformLineToMachineCode(std::vector<char>&, std::string, std::map<int, std::string>&, int);
bool writeMachineCodeToFile(std::vector<char>&, std::string);
void createHeader(std::vector<char>&, std::vector<char>&, uint16_t);

bool fixLabelJumpPoints(std::vector<char>&, std::map<std::string, int>&, std::map<int, std::string>, std::vector<int>, const unsigned); 

bool preprocess(const std::string& infile, const std::string& outfile);

bool isReg(char);
char regToNum(char);
#endif
