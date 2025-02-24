#include "Util.hpp"

// checks if a string of character is a number. Supports negative, scientific, and decimals
bool util::isNumber(const char* pattern) {
	std::regex Rexp("^[+-]?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$");
	return std::regex_match(pattern, Rexp);
}
bool util::isNumber(char pattern) {
	const char* cpChar = &pattern;
	return util::isNumber(cpChar);
}
