#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include "Token.hpp"
#include "../Util.hpp"

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
	void compileFile(const char* lumeFileDirr);
};

#endif // !COMPILER_HPP
