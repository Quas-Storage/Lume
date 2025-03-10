#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include "../Lib/Error.hpp"
#include "./ParserToken.hpp"
#include "./astNodes.hpp"

using namespace std;

enum PAR_associatives : int {
	left,
	right,
};

struct opInfo {
	uint8_t precedence;
	PAR_associatives ass;
};

// PAR

class parser {
public:
	parser(vector<token>* tokArr);

	void genAst();
	vector<astNode> getAst();

	void displayAst();
private:
	vector<parserToken> tokArr;

	vector<astNode> parserTree;
	parserToken* currTok;
	unsigned int currCaretPos;
	unsigned int arrLength;

	void shiftCaret(int8_t steps);
	bool isAtEnd();

	// AST FUNCS

	variant<numNode, branchNode> compAtom(branchNode* branch);
	binNode compOperator(parserToken* op,
		variant<numNode*, branchNode*, nullptr_t> L_atom,
		variant<numNode*, branchNode*, nullptr_t> R_atom);
	binNode compOperator(parserToken* op,
		variant<numNode*, branchNode*, nullptr_t> L_atom,
		variant<numNode, branchNode> R_atom);
	void compExpression(uint8_t minPrecedence, branchNode* branch);

	void parseEOL(branchNode* branch, parserToken* tok);
	void parseLine(branchNode* branch);
};

class parserHelper {
public:
	static bool tokenIsInt(tokenType type);
};

#endif