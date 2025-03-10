#include "./parser.hpp"

const map<string, opInfo> binOpMap = {
	{"+", opInfo{1, PAR_associatives::left}},
	{"-", opInfo{1, PAR_associatives::left}},
	{"*", opInfo{2, PAR_associatives::left}},
	{"/", opInfo{2, PAR_associatives::left}},
	{"%", opInfo{2, PAR_associatives::left}},
	{"^", opInfo{3, PAR_associatives::right}},
};

parser::parser(vector<token>* tokArr) {
	for (token tok : *tokArr) {
		parserToken parTok = parserToken(&tok);
		this->tokArr.push_back(parTok);
	}

	this->currCaretPos = 0;
	this->currTok = &(this->tokArr.at(0));
	this->arrLength = static_cast<unsigned int>(this->tokArr.size());
}

void parser::shiftCaret(int8_t steps) {
	this->currCaretPos += steps;
	if (this->isAtEnd())
		return;
	this->currTok = &(this->tokArr.at(this->currCaretPos));
}

bool parser::isAtEnd() {
	return this->currCaretPos >= this->arrLength;
}

vector<astNode> parser::getAst() {
	return this->parserTree;
}

void parser::genAst() {
	if (this->arrLength == 0)
		return;

	branchNode baseBranch;
	this->parseLine(&baseBranch);
}

variant<numNode, branchNode> parser::compAtom(branchNode* branch) {
	parserToken* currTok = this->currTok;
	tokenType tokType = currTok->getType();

	if (tokType == tokenType::NULL_TOKEN) {
		throw runtime_error("Current Token is undefined at position" + currTok->getPos());
	}
	else if (tokType == tokenType::TT_leftParen) {
		this->shiftCaret(1);
		currTok->setRead(true);

		branchNode leftAtom;
		this->compExpression(1, &leftAtom);

		tokenType shiftTokType = this->currTok->getType();
		if (shiftTokType == tokenType::TT_rightParen || shiftTokType == tokenType::TT_rightParen && this->currTok->getRead()) {
			unsigned int tokPos = currTok->getPos();
			syntaxError(&tokPos, currTok->getPos() + 2, "Parathesized expression not closed. Expected ')'", errorConfig{
				true,
				processorType::LUME_compiler,
				});
		}
		this->currTok->setRead(true);
		this->shiftCaret(1);
		branch->addNode(&leftAtom);

		return leftAtom;
	}
	else if (tokType == tokenType::TT_binOp) {
		// for expression that arent negatively paranthesized (aka -(10 + 10) )
		if (currTok->getValue() == "-") {
			currTok->setRead(true);
			this->shiftCaret(1);

			branchNode subBranch;
			variant<numNode, branchNode> rightAtom = this->compAtom(&subBranch);
			binNode B_node = this->compOperator(currTok, nullptr, rightAtom);

			branch->addNode(&B_node);

			if (holds_alternative<numNode>(rightAtom)) {
				numNode nod = get<numNode>(rightAtom);
				branch->addNode(&nod);
			}
			else {
				branchNode nod = get<branchNode>(rightAtom);
				branch->addNode(&nod);
			}
			return rightAtom;
		}

		unsigned int tokPos = currTok->getPos();
		syntaxError(&tokPos, currTok->getPos() + 1, "Unexpected binary operator '" + currTok->getValue() + "' in expression. Expected a valid operator like '+', '-', '*', or '/'. ", errorConfig{
			true,
			processorType::LUME_compiler,
			});
	}
	else {
		currTok->setRead(true);
		this->shiftCaret(1);

		switch (currTok->getType()) {
		case tokenType::TT_int8: {
			numNode node = numNode(astType::AST_int8, currTok);
			branch->addNode(&node);
			return node;
		}
		case tokenType::TT_int16: {
			numNode node = numNode(astType::AST_int16, currTok);
			branch->addNode(&node);
			return node;
		}
		case tokenType::TT_int32: {
			numNode node = numNode(astType::AST_int32, currTok);
			branch->addNode(&node);
			return node;
		}
		case tokenType::TT_int64: {
			numNode node = numNode(astType::AST_int64, currTok);
			branch->addNode(&node);
			return node;
		}
		case tokenType::TT_float: {
			numNode node = numNode(astType::AST_float, currTok);
			branch->addNode(&node);
			return node;
		}
		}
	}
}

binNode parser::compOperator(parserToken* op,
	variant<numNode*, branchNode*, nullptr_t> L_atom,
	variant<numNode*, branchNode*, nullptr_t> R_atom) {
	string val = op->getValue();
	if (val == "+")							return binNode(astType::AST_add, op, L_atom, R_atom);
	if (val == "-")							return binNode(astType::AST_sub, op, L_atom, R_atom);
	if (val == "*")							return binNode(astType::AST_mul, op, L_atom, R_atom);
	if (val == "/")							return binNode(astType::AST_div, op, L_atom, R_atom);
	if (val == "^")							return binNode(astType::AST_pow, op, L_atom, R_atom);
	if (val == "%")							return binNode(astType::AST_mod, op, L_atom, R_atom);

	return binNode(astType::AST_add, op, L_atom, R_atom);
};
binNode parser::compOperator(parserToken* op,
	variant<numNode*, branchNode*, nullptr_t> L_atom,
	variant<numNode, branchNode> R_atom) {
	string val = op->getValue();
	if (val == "+") return					binNode(astType::AST_add, op, L_atom, R_atom);
	if (val == "-") return					binNode(astType::AST_sub, op, L_atom, R_atom);
	if (val == "*") return					binNode(astType::AST_mul, op, L_atom, R_atom);
	if (val == "/") return					binNode(astType::AST_div, op, L_atom, R_atom);
	if (val == "^") return					binNode(astType::AST_pow, op, L_atom, R_atom);
	if (val == "%") return					binNode(astType::AST_mod, op, L_atom, R_atom);

	return binNode(astType::AST_add, op, L_atom, R_atom);
};


void parser::compExpression(uint8_t minPrecedence, branchNode* branch) {
	variant<numNode, branchNode> leftAtom = this->compAtom(branch);

	while (this->currTok->getType() == tokenType::TT_binOp) {
		string tokVal = this->currTok->getValue();

	}
}

void parser::parseEOL(branchNode* branch, parserToken* tok) {
	eolNode endLineNode = eolNode(astType::AST_eol, tok);
	branch->addNode(&endLineNode);

	branchNode subBranch;
	this->shiftCaret(1);
	this->parseLine(&subBranch);
	if (subBranch.length != 0) {
		branch->addNode(&subBranch);
	}
}

void parser::parseLine(branchNode* branch) {
	parserToken* currTok = this->currTok;
	unsigned int currPos = this->currCaretPos;

	while (!this->isAtEnd() && currTok->getType() != tokenType::TT_eol) {
		tokenType currTokType = currTok->getType();

		if (currTokType == tokenType::TT_binOp || parserHelper::tokenIsInt(currTokType) || currTokType == tokenType::TT_float) {
			branchNode exprBranch;
			this->compExpression(0, &exprBranch);
			this->shiftCaret(-1);
			branch->addNode(&exprBranch);
		}

		this->shiftCaret(1);
		currTok = this->currTok;
		currTokType = currTok->getType();

		if (currTokType == tokenType::TT_eol) {
			this->parseEOL(branch, currTok);
		}
		else if (currTokType == tokenType::TT_rightParen && !currTok->getRead()) {
			unsigned int currPos = this->currTok->getPos();
			syntaxError(&currPos, currPos + 1, "closing parenthesis ')' does not match opening parenthesis '('", errorConfig{
				true,
				processorType::LUME_compiler
				});
		}
	}
}


////////////////////////////////////////////////////////
// parser helper funcs
////////////////////////////////////////////////////////


bool parserHelper::tokenIsInt(tokenType type) {
	return type == tokenType::TT_int8
		|| type == tokenType::TT_int16
		|| type == tokenType::TT_int32
		|| type == tokenType::TT_int64;
}