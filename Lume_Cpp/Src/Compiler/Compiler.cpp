#include "Compiler.hpp"

// creates a new compiler instant
compiler::compiler(const char* filePath, compilerOptions* options) {
	this->filePath = filePath;
	this->Options = options;
}

// quick method for reading file content
std::string compiler::readTextFile(const char* fileDirr) {
	std::ifstream file(fileDirr);
	std::string fileContents;
	const std::string dirr = static_cast<const std::string>(fileDirr);

	if (!file.is_open()) {
		throw std::runtime_error(dirr + "failed to open");
	}
	else if (file.bad()) {
		throw std::runtime_error(dirr + "failed to open, bad bit error");
	}
	else if (file.fail()) {
		throw std::runtime_error(dirr + "failed to open, failed bit error");
	}

	if (file.good()) {
		std::vector<char> buffer(FILE_BUFFER_SIZE);
		std::istringstream iss;

		while (file.read(buffer.data(), FILE_BUFFER_SIZE)) {
			std::streamsize bytesRead = file.gcount();
			iss.str(std::string(buffer.data(), bytesRead));
			iss.clear();

			std::string line;
			while (getline(iss, line)) {
				if (!line.empty()) {
					fileContents.append(line);
				}
			}
		}

		std::streamsize bytesRead = file.gcount();
		if (bytesRead > 0) {
			std::string lastChunk(buffer.data(), bytesRead);
			fileContents.append(lastChunk);
		}

		file.close();
	}
	else {
		const std::string dirr = static_cast<const std::string>(fileDirr);
		throw std::runtime_error(dirr + "Failed to process file");
	}

	return fileContents;
}

// compiles the selected lume file and executes it
void compiler::compileFile(const char* lumeFileDirr) {
	std::string fileContent = this->readTextFile(lumeFileDirr);
	
	lexer lexerInstance = lexer(fileContent);
	lexerInstance.lex();
	std::vector<token> tokenArray = lexerInstance.getLexedTokens();
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

	if (this->Options->debug && !status.status) {
		throw std::runtime_error("Exited compilation with error with message \n" + static_cast<std::string>(status.errMessage));
	}

	return status;
}

