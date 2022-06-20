#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <iostream>
#include <iomanip>
#include <string>

#define HEX( x ) std::setw(2) << std::setfill('0') << std::hex << (0xFF & x)

bool assemble(std::string);

int rewriteNum(std::string);


#endif
