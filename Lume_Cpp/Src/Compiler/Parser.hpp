#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <variant>
#include <vector>
#include <map>
#include "./Token.hpp"
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

class parserToken : token {
public:
	bool getRead();
private:
	bool read;
};

// PAR

class parser {
public:
	parser(vector<token> tokArr);

	void genAst();
	vector<astNode> getAst();

	void displayAst();
private:
	vector<parserToken> tokArr;

	vector<astNode> parserTree;
	parserToken currTok;
	unsigned int currCaretPos;

	void shiftCaret(uint8_t steps);

	// AST FUNCS

	variant<numNode, branchNode> compAtom(branchNode* branch);
	binNode compOp(parserToken* op,
		variant<numNode*, branchNode*> L_atom,
		variant<numNode*, branchNode*> R_atom);
	branchNode compExpr(uint8_t minPrecedence, branchNode* branch);
};

#endif