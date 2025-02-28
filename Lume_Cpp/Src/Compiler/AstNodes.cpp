#include "./AstNodes.hpp"

astNode::astNode(token* tok) {
	this->tok = tok;
}

token* astNode::getTok() {
	return this->tok;
}

string astNode::toString() {
	tokenType tokType = this->tok->getType();
	string tokTypeStr = this->tok->tokenTypeToString(&tokType);
	string tokStr = tokTypeStr + " : " + this->tok->getValue();
	return tokStr;
}

string astNode::astTypeToStr(astType type) {
    switch (type) {
    case AST_int8:      return "int8";
    case AST_int16:     return "int16";
    case AST_int32:     return "int32";
    case AST_int64:     return "int64";
    case AST_float:     return "float";
    case AST_mul:       return "mul";
    case AST_div:       return "div";
    case AST_add:       return "add";
    case AST_sub:       return "sub";
    case AST_mod:       return "mod";
    case AST_pow:       return "pow";
    case AST_eol:       return "eol";
    default:            return "Unknown";
    }
}

////////////////////////////////////////////////////////
// Ast tok Types
////////////////////////////////////////////////////////

void branchNode::addNode(astNode* node) {
    this->nodes.push_back(node);
}

numNode::numNode(astType astType, token* tok) 
    : astNode(tok) {
    this->tok = tok;
    this->type = astType;
}

binNode::binNode(astType astType, token* tok, binOpFacNode left, binOpFacNode right)
    : astNode(tok) {
    this->tok = tok;
    this->type = astType;
    this->left = left;
    this->right = right;
}