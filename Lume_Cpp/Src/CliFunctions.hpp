#ifndef CLI_FUNCTIONS_HPP
#define CLI_FUNCTIONS_HPP
#define LUME_VERSION "V1.00.1";

#include <iostream>
#include <string>
#include <algorithm>
#include "Util.hpp"
#include "Compiler/Compiler.hpp"

void lumeWelcomeMessage();
void lumeHelp();
void lumeVersion();
void runProgram(const char* filePath, bool debugMode);

#endif