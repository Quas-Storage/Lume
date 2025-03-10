#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <iostream>
#include <variant>
#include <vector>
#include "./token.hpp"

using namespace std;

enum astType {
	AST_int8,	AST_int16,	
	AST_int32,	AST_int64,
	AST_float,
	
	AST_mul,	AST_div,
	AST_add,	AST_sub,
	AST_mod,	AST_pow,

	AST_eol,
};

class astNode {
public:
	static string astTypeToStr(astType type);
	
	token* getTok();

	string toString();
	astNode(); // for NULL tokens
	astNode(token* tok);
private:
	token* tok;
};

////////////////////////////////////////////////////////
// Ast tok Types
////////////////////////////////////////////////////////

class numNode : public astNode {
public:
	astType type;
	token* tok;

	numNode(astType astType, token* tok);
};

class branchNode : public astNode {
public:
	vector<astNode*> nodes;
	unsigned int length;

	branchNode();
	void addNode(astNode* node);
};

typedef variant<numNode*, branchNode*, nullptr_t> binOpFacNode;
typedef variant<numNode, branchNode> binOpFacNodeExt;
class binNode : public astNode {
public:
	astType type;
	token* tok;
	binOpFacNode left;
	variant<binOpFacNode, binOpFacNodeExt> right;

	binNode(astType astType, token* tok, binOpFacNode left, variant<binOpFacNode, binOpFacNodeExt> right); // auto stands for numNode or either ASTbranch
};

class eolNode : public astNode {
public:
	astType type;
	token* tok;

	eolNode(astType astType, token* tok);
};

#endif