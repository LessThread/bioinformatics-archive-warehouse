#pragma once
#include "PublicHeader.h"

void FileRead_and_BreakUp();
std::string getBaseSequence(char* str, unsigned id);
void writeFile(std::string[], unsigned id, unsigned count);
void mergeFile(std::string[]);
void mergeFile(unsigned);