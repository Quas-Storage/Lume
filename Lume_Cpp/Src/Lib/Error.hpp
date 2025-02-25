#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include "../Util.hpp"

using namespace std;

enum errorType : int {
	ERR_undefinedCharacter,
	ERR_mallformedInteger,
	ERR_mallformedFloat,
	ERR_syntaxError,
	ERR_invalidOperation,
};

enum processorType : int {
	PT_compiler,
	PT_runTime,
};

struct errorConfig {
	bool genStackTrace;
	processorType processorType;
};

const inline char* currentLumeFile; // current file content being proccessed currently

class error {
public:
	error(unsigned int* posStart, unsigned int posEnd, errorType errCode, string details, errorConfig config);

	static const char* procToString(processorType procType);
	static const char* errToString(errorType errType);
private:
	void throwErr();
	vector<string> genStackTrace();

	unsigned int* posStart;
	unsigned int posEnd;
	errorType errCode;
	string details;
	errorConfig config;
};

class mallformedInteger : error {
public:
	mallformedInteger(unsigned int* posStart, unsigned int posEnd, string details, errorConfig config);
};

#endif