#ifndef LEXER_HPP
#define LEXER_HPP

#define BIN_IDENTIFIERS "+-/*^%";
#define LEXICAL_IDENTIFIERS "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include "../Util.hpp"
#include "../Lib/Error.hpp"
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

	struct numExtRes {
		string str;
		unsigned int size;
	};

	string sourceFile;
	vector<token> tokens;

	unsigned int caretIndex;
	size_t fileLength;
	char currentToken;

	void advance(int steps);
	bool isAtEnd();
	const char getTokAtIndex(unsigned int index);

	numExtRes getNumExtends(unsigned int index);
	tokenType getIntCategory(string* num, unsigned int* index, unsigned int* size);
	string checkBinSyntax(unsigned int index);
	
	token createToken(tokenType type, string value);
	scanResult scanTokenAtIndex();
	
};

class lexerHelper {
public:
	static void condenseStr(string* str);
	static vector<string> splitStrOnSym(string* str);
	static long long convBigNumStr(string* str);
	static bool isBinOp(char binOp);
	static bool atIndexIsValid(string* source, unsigned int index);
};


#endif // !LEXER_HPP
