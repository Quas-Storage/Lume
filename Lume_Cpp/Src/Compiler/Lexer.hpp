#ifndef LEXER_HPP
#define LEXER_HPP

#define BIN_IDENTIFIERS "+-/*^%";
#define LEXICAL_IDENTIFIERS "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include "../Util.hpp"
#include "Token.hpp"

const char* identifiers[];
using namespace std;

class lexer {
public:
	lexer(string fileContent);

	void lex();
	vector<token> getLexedTokens();
private:
	struct scanResult {
		token tokenInst;
		unsigned int strideSize;
	};

	string sourceFile;
	vector<token> tokens;

	unsigned int carrotIndex;
	unsigned int fileLength;
	char currentToken;

	void advance(int steps);
	bool isBinOp(char binOp);
	bool isAtEnd();
	const char getTokAtIndex(unsigned int index);

	string getNumExtends(unsigned int index);
	
	token createToken(tokenType type, const char* value);
	scanResult scanTokenAtIndex();
};


#endif // !LEXER_HPP
