#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include "cpu.h"

int disassemble(const std::string& file);
void instructionName(std::array<uint8_t,4> pcb, uint8_t pos);
bool pcbIsValidIns(std::array<uint8_t,4> pcb, uint8_t pos);


char regNum(int r);



#endif
