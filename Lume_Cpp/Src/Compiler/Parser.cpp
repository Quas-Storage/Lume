#include "./parser.hpp"

// returns the state to check if the token has 
// already been read by the parser
bool parserToken::getRead() {
	return this->read;
}

////////////////////////////////////////////////////////
// parser shit
////////////////////////////////////////////////////////

const map<string, opInfo> binOpMap = {
	{"+", opInfo{1, PAR_associatives::left}},
	{"-", opInfo{1, PAR_associatives::left}},
	{"*", opInfo{2, PAR_associatives::left}},
	{"/", opInfo{2, PAR_associatives::left}},
	{"%", opInfo{2, PAR_associatives::left}},
	{"^", opInfo{3, PAR_associatives::right}},
};