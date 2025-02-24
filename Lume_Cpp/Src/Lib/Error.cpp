#include "Error.hpp"

void error::throwErr() {

}

error::error(unsigned int* posStart, unsigned int* posEnd, errorType errCode, string details, errorConfig config) {

}

string* error::genStackTrace() {
	string hi[] = { "sds", "hi" };
	return hi;
}


// error types


mallformedInteger::mallformedInteger(unsigned int* posStart, unsigned int* posEnd, errorType errCode, string details, errorConfig config) : error {
}