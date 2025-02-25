#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <regex>

using namespace std;

namespace util {
	bool isNumber(char pattern);
	bool isNumber(const char* pattern);

	vector<string> strSplit(string* str, string* delim);
	vector<string> strSplit(string* str, string delim);
	vector<string> strSplit(const char* str, const char* delim);
	vector<string> strSplit(string* str, regex regExpr);
};

#endif // !UTIL_HPP
