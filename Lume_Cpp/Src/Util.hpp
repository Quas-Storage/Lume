#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <regex>
#include <variant>
#include <stdexcept>

using namespace std;

namespace util {
	bool isNumber(char pattern);
	bool isNumber(const char* pattern);

	bool isFloat(const char* num);
	bool isFloat(string* num);

	bool isInt(const char* num);
	bool isInt(string* num);

	vector<string> strSplit(string* str, string* delim);
	vector<string> strSplit(string* str, string delim);
	vector<string> strSplit(const char* str, const char* delim);
	vector<string> strSplit(string* str, regex regExpr);

	string toUpper(string str);
	string toUpper(const char* str);
};

#endif // !UTIL_HPP
