#include "Error.hpp"

void error::throwErr() {
	if (!currentLumeFile)
		throw runtime_error("lume file not specified!");

	const vector<string> progLines = util::strSplit(currentLumeFile, "\n");
	unsigned int currentCaretPos = 0;
	unsigned int lastCaretPos = 0;
	unsigned int lineNum = 0;
	for (lineNum; lineNum < progLines.size(); lineNum++) {
		currentCaretPos += abs(static_cast<int>(progLines.at(lineNum).length() + 1)); // add one to it because idk why? it just works?
		if (*(this->posStart) <= currentCaretPos) {
			lineNum++;// fix the offset cuz editors start at line one, not zero
			break;
		}
		lastCaretPos = currentCaretPos;
	}

	string strlumeFile = static_cast<string>(currentLumeFile);
	string middleCode = strlumeFile.substr(*(this->posStart), this->posEnd - *(this->posStart));
	string beginCode = strlumeFile.substr(lastCaretPos, *(this->posStart) - lastCaretPos);
	string endCode = strlumeFile.substr(this->posEnd, currentCaretPos - 1 - this->posEnd);
	const char* procTypeStr = error::procToString(config.processorType);
	const char* errString = error::errToString(this->errCode);

	regex regExpr("[\\x20-\\x7E]");
	string emptBegin = regex_replace(beginCode, regExpr, " ");;
	string cursoredMiddle = regex_replace(middleCode, regExpr, "^");;

	string errMessage = "\x1b[91m" + static_cast<string>(procTypeStr) + " ERROR; Line:" + to_string(lineNum) + " ";
	errMessage += "Code:" + to_string(this->errCode) + " ";
	errMessage += static_cast<string>(errString) + "\n\n";
	errMessage += beginCode + middleCode + endCode + "\n";
	errMessage += emptBegin + cursoredMiddle + "\n";
	errMessage += this->details + "\x1b[0m";
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	cout << errMessage << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;

	exit(this->errCode);
}

error::error(unsigned int* posStart, unsigned int posEnd, errorType errCode, string details, errorConfig config) {
	this->config = config;
	this->details = details;
	this->errCode = errCode;
	this->posEnd = posEnd;
	this->posStart = posStart;

	this->throwErr();
}

const char* error::procToString(processorType procType) {
	switch (procType) {
	case processorType::PT_compiler:
		return "compiler";
	case processorType::PT_runTime:
		return "runtime";
	default:
		throw runtime_error("Invalid process type" + procType);
	}
}

const char* error::errToString(errorType errType) {
	switch (errType) {
	case errorType::ERR_mallformedFloat:
		return "mallformedFloat";
	case errorType::ERR_mallformedInteger:
		return "mallformedInteger";
	case errorType::ERR_invalidOperation:
		return "invalidOperation";
	case errorType::ERR_syntaxError:
		return "syntaxError";
	case errorType::ERR_undefinedCharacter:
		return "undefinedCharacter";
	default:
		throw runtime_error("Invalid error type " + errType);
	}
}


vector<string> error::genStackTrace() {
	// still todo when Multi files get added
}

////////////////////////////////////////////////////////
// error types
////////////////////////////////////////////////////////

mallformedInteger::mallformedInteger(unsigned int* posStart, unsigned int posEnd, string details, errorConfig config) 
	: error(posStart, posEnd, errorType::ERR_mallformedInteger, details, config) {
}