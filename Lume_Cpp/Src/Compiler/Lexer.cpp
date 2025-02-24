#include "Lexer.hpp"

const char* identifiers[] = {
	// syntax crap
	"(",
	")",
	";",
	".",
	// bin operators
 	"+", "-", "*", 
	"/", "^", "%"
};

lexer::lexer(string fileContent) {
	this->sourceFile = fileContent;
	this->carrotIndex = 0;
	this->tokens;
	this->fileLength = fileContent.length();
	this->currentToken = fileContent.at(0);
}

void lexer::advance(int steps) {
	this->carrotIndex += steps;
	if (this->isAtEnd()) 
		return;
	this->currentToken = this->sourceFile.at(this->carrotIndex);
}

const char lexer::getTokAtIndex(unsigned int index) {
	return this->sourceFile.at(index);
}

bool lexer::isAtEnd() {
	return this->carrotIndex >= static_cast<int>(this->fileLength);
}
		
bool lexer::isBinOp(char binOp) {
	string BID = BIN_IDENTIFIERS;
	return BID.find(binOp) != string::npos;
}

token lexer::createToken(tokenType type, const char* value) {
	return token{
		value, 
		type, 
		this->carrotIndex 
	};
}

// return the lexed tokens if lexed correctly
vector<token> lexer::getLexedTokens() {
	return this->tokens;
}

// does the lexing and stuff
void lexer::lex() {
	if (this->fileLength == 0) return;

	while (!this->isAtEnd()) {
		lexer::scanResult scanRes = this->scanTokenAtIndex();
		this->advance(scanRes.strideSize);

		if (scanRes.tokenInst.getType() == tokenType::NULL_TOKEN) continue;
		this->tokens.push_back(scanRes.tokenInst);
	}
}

// scan the token at a current index
lexer::scanResult lexer::scanTokenAtIndex() {
	const char CT = this->currentToken;
	unsigned int CI = this->carrotIndex;
	
	if (CT == NULL)
		throw runtime_error("Token Undefined at carrot position " + this->carrotIndex);
	if (CT == ' ')
		return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, NULL), 1 };

	if (util::isNumber(CT) || CT == '.' && util::isNumber(this->getTokAtIndex(CI + 1)) || CT == '-' && util::isNumber(this->getTokAtIndex(CI + 1))) {
		string numExt(this->getNumExtends(CI));
	}

	return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, NULL), 1 };
}

string lexer::getNumExtends(unsigned int index) {
	string croppedSource(this->sourceFile.substr(index));
	string numExt;
	regex regExpr("[^\w\.]"); // split at each symbol apart from a dot

	// if number with minus, then cut it off becuase regex expr 
	// filters out syms, then add it back on later
	// otherwise just regex filter the text
	if (croppedSource.find_first_of("-") == 0) {
		string crop(croppedSource.substr(1));

		regex_replace(crop, regExpr, "$1");
		crop.insert(0, "-");
		numExt = crop;
	}
	else {
		string cropBuffer(croppedSource);
		regex_replace(cropBuffer, regExpr, "$1");
		numExt = cropBuffer;
	}

	int tally = ranges::count(numExt, '.');

	cout << tally << endl;
}