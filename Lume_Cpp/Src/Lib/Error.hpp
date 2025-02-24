#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

using namespace std;

enum errorType : int {
	undefinedCharacter,
	mallformedInteger,
	mallformedFloat,
	syntaxError,
	invalidOperation,
};

enum processorType : int {
	compiler,
	runTime,
};

struct errorConfig {
	bool genStackTrace;
	processorType processorType;
};

class error {
public:
	error(unsigned int* posStart, unsigned int* posEnd, errorType errCode, string details, errorConfig config);
private:
	void throwErr();
	std::string* genStackTrace();
};

class mallformedInteger : error {
public:
	mallformedInteger(unsigned int* posStart, unsigned int* posEnd, errorType errCode, string details, errorConfig config);
};

#endif