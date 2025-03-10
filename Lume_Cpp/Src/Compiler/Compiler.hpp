#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Lexer.hpp"
#include "Token.hpp"
#include "parser.hpp"
#include "AstNodes.hpp"
#include "../Util.hpp"
#include "../Lib/Error.hpp"

#define FILE_BUFFER_SIZE 1024

using namespace std;

struct compilerStatus
{
	bool status;
	const char* errMessage;
};

struct compilerOptions 
{
	bool debug;
};

class compiler {
public:
	const char* filePath;
	const compilerOptions* Options;

	compiler(const char* File, compilerOptions* options);
	compilerStatus compile();
private:
	std::string readTextFile(const char* fileDirr);
	void compileFile(const char* lumeFileDirr);
};

#endif // !COMPILER_HPP
