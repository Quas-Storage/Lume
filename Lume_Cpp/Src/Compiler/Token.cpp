#include "Token.hpp"

// creates a new token
token::token(const char* value, tokenType type, unsigned int pos) {
	this->type = type;
	this->pos = pos;
	this->value = value;
}

// converts the tokentype to string
const char* token::tokenTypeToString(tokenType* type) {
	switch (*type) {
	case TT_int8:
		return "int8";
	case TT_int16:
		return "int16";
	case TT_int32:
		return "int32";
	case TT_int64:
		return "int64";
	case TT_float:
		return "float";
	default:
		throw runtime_error("invalid tokentype");
	};
}

// returns the type
tokenType token::getType() {
	return this->type;
}

// returns the value
const char* token::getValue() {
	return this->getValue();
}

// return the carrot position of token
unsigned int token::getPos() {
	return this->pos;
}

// returns the token itself
tokenInst token::getToken() {
	tokenInst tokenInstance;
	tokenInstance.pos = this->pos;
	tokenInstance.type = this->type;
	tokenInstance.value = this->value;

	return tokenInstance;
}

// converts the token to string for quick displaying
const char* token::toString() {
	const char* tokenString = this->tokenTypeToString(&(this->type));
	string tokenTypeString = static_cast<std::string>(tokenString) + ": " + this->value;
	const char* tokenCharString = tokenTypeString.c_str();
	
	return tokenCharString;
}