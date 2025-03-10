#include "./ParserToken.hpp"

parserToken::parserToken(token* tok)
	: token(tok->getValue(), tok->getType(), tok->getPos()) {
	this->read = false;
}

parserToken::parserToken(string value, tokenType type, unsigned int pos) : 
	token(value, type, pos) {
	this->read = false;
}

// returns the state to check if the token has 
// already been read by the parser
bool parserToken::getRead() {
	return this->read;
}

void parserToken::setRead(bool s) {
	this->read = s;
}
