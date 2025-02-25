#include "Util.hpp"

// checks if a string of character is a number. Supports negative, scientific, and decimals
bool util::isNumber(const char* pattern) {
	regex Rexp("^[+-]?\\d+(\\.\\d+)?([eE][+-]?\\d+)?$");
	return regex_match(pattern, Rexp);
}
bool util::isNumber(char pattern) {
	string hi(1, pattern);
	return util::isNumber(hi.c_str());
}

// splits string with given character
vector<string> util::strSplit(string* str, string* delim) {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = (*str).find(*delim)) != string::npos) {
        token = (*str).substr(0, pos);
        tokens.push_back(token);
        (*str).erase(0, pos + (*delim).length());
    }
    tokens.push_back(*str);

    return tokens;
}
vector<string> util::strSplit(const char* str, const char* delim) {
    string strCast = static_cast<string>(str);
    string delimCast = static_cast<string>(delim);
    return util::strSplit(&strCast, &delimCast);
}
vector<string> util::strSplit(string* str, string delim) {
    string delimCast = static_cast<string>(delim);
    return util::strSplit(str, &delimCast);
}
vector<string> util::strSplit(string* str, regex regExpr) {
    vector<string> elems;
    sregex_token_iterator iter((*str).begin(), (*str).end(), regExpr, -1);
    sregex_token_iterator end;

    while (iter != end) {
        if (iter->length()) {
            elems.push_back(*iter);
        }
        ++iter;
    }

    return elems;
}