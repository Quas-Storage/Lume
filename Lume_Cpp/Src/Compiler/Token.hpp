#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

enum tokenType : int {
	TT_int8,
	TT_int16,
	TT_int32,
	TT_int64,
	TT_float,
	TT_string,
};

struct tokenInst {
	const char* value;
	tokenType type;
	unsigned int pos;
};

class token {
public:
	token(const char* value, tokenType type, unsigned int pos);
	const char* toString();

	tokenInst getToken();
	tokenType getType();
	const char* getValue();
	unsigned int getPos();

	static const char* tokenTypeToString(tokenType* type);
private:
	unsigned int pos;
	const char* value;
	tokenType type;
};

#endif // !TOKEN_HPP
