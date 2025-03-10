#include "Compiler.hpp"

// creates a new compiler instant
compiler::compiler(const char* filePath, compilerOptions* options) {
	this->filePath = filePath;
	this->Options = options;
}

// quick method for reading file content
string compiler::readTextFile(const char* fileDirr) {
	ifstream file(fileDirr);
	string fileContents;
	const string dirr = static_cast<const string>(fileDirr);

	if (!file.is_open()) {
		throw runtime_error(dirr + "failed to open");
	}
	else if (file.bad()) {
		throw runtime_error(dirr + "failed to open, bad bit error");
	}
	else if (file.fail()) {
		throw runtime_error(dirr + "failed to open, failed bit error");
	}

	if (file.good()) {
		vector<char> buffer(FILE_BUFFER_SIZE);
		istringstream iss;

		while (file.read(buffer.data(), FILE_BUFFER_SIZE)) {
			streamsize bytesRead = file.gcount();
			iss.str(string(buffer.data(), bytesRead));
			iss.clear();

			string line;
			while (getline(iss, line)) {
				if (!line.empty()) {
					fileContents.append(line);
				}
			}
		}

		streamsize bytesRead = file.gcount();
		if (bytesRead > 0) {
			string lastChunk(buffer.data(), bytesRead);
			fileContents.append(lastChunk);
		}

		file.close();
	}
	else {
		const string dirr = static_cast<const string>(fileDirr);
		throw runtime_error(dirr + "Failed to process file");
	}

	return fileContents;
}

// compiles the selected lume file and executes it
void compiler::compileFile(const char* lumeFileDirr) {
	string fileContent = this->readTextFile(lumeFileDirr);
	currentLumeFile = fileContent.c_str();

	lexer lexerInstance = lexer(fileContent);
	lexerInstance.lex();
	vector<token> tokenArray = lexerInstance.getLexedTokens();

	for (token _T : tokenArray) {
		cout << _T.toString() << endl;
	}

	parser parserInstance = parser(&tokenArray);
	parserInstance.genAst();
	vector<astNode> astTree = parserInstance.getAst();
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
	catch (const exception& err)
	{
		status.status = false;
		status.errMessage = err.what();
	}

	if (this->Options->debug && !status.status) {
		throw runtime_error("Exited compilation with error with message \n" + static_cast<string>(status.errMessage));
	}

	return status;
}

