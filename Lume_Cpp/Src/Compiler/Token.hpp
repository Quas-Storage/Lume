#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

using namespace std;

enum tokenType : int {
	TT_int8,
	TT_int16,
	TT_int32,
	TT_int64,
	TT_float,
	TT_leftParen,
	TT_rightParen,
	TT_binOp,
	TT_eol,
	NULL_TOKEN,
};

struct tokenInst {
	string value;
	tokenType type;
	unsigned int pos;
};

class token {
public:
	token(string value, tokenType type, unsigned int pos);
	string toString();

	tokenInst getToken();
	tokenType getType();
	const char* getValue();
	unsigned int getPos();

	static const char* tokenTypeToString(tokenType* type);
private:
	unsigned int pos;
	string value;
	tokenType type;
};

#endif // !TOKEN_HPP
