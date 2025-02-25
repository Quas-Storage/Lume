#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <vector>
#include <filesystem>
#include "CliFunctions.hpp"
#include "Util.hpp"

using namespace std;

bool argExists(vector<string>* flags, const char* pattern);
void processArgs(char* argv[]);

unsigned int runCli(char* argv[]);

#endif // !CLI_HPP
