#ifndef PARSER_TOKEN_HPP
#define PARSER_TOKEN_HPP

#include <iostream>
#include "./Token.hpp"

using namespace std;

class parserToken : public token {
public:
	parserToken(token* tok);
	parserToken(string value, tokenType type, unsigned int pos);

	bool getRead();
	void setRead(bool s);
private:
	bool read;
};

#endif