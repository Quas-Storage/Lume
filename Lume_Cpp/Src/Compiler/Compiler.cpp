#include "Compiler.hpp"

// creates a new compiler instant
compiler::compiler(const char* filePath, compilerOptions* options) {
	this->filePath = filePath;
	this->Options = options;
}

// compiles the selected lume file and executes it
void compiler::compileFile(const char* lumeFileDirr) {

}

// Handles the compiler
compilerStatus compiler::compile() {
	compilerStatus status;
	status.status = true;
	status.errMessage = "";

	try
	{
		this->compileFile(this->filePath);
	}
	catch (const std::exception& err)
	{
		status.status = false;
		status.errMessage = err.what();
	}

	if (this->Options->debug) {
		std::cerr << "Exited compilation with error with message \n" + static_cast<std::string>(status.errMessage) << std::endl;
	}

	return status;
}

