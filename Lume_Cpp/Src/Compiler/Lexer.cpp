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
	this->caretIndex = 0;
	this->tokens;
	this->fileLength = fileContent.length();
	this->currentToken = fileContent.at(0);
}

void lexer::advance(int steps) {
	this->caretIndex += steps;
	if (this->isAtEnd()) 
		return;
	this->currentToken = this->sourceFile.at(this->caretIndex);
}

const char lexer::getTokAtIndex(unsigned int index) {
	return this->sourceFile.at(index);
}

bool lexer::isAtEnd() {
	return this->caretIndex >= static_cast<int>(this->fileLength);
}
		
bool lexer::isBinOp(char binOp) {
	string BID = BIN_IDENTIFIERS;
	return BID.find(binOp) != string::npos;
}

void lexer::condenseStr(string* str) {
	(*str).erase(remove_if((*str).begin(), (*str).end(), [](unsigned char _C) {
		return isspace(_C) || _C == '\n' || _C == '\r';
		}), (*str).end());
}

token lexer::createToken(tokenType type, const char* value) {
	return token{
		value, 
		type, 
		this->caretIndex 
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
	unsigned int CI = this->caretIndex;

	if (CT == NULL)
		throw runtime_error("Token Undefined at caret position " + this->caretIndex);
	if (CT == ' ')
		return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, NULL), 1 };

	if (util::isNumber(CT) 
		|| CT == '.' && util::isNumber(this->getTokAtIndex(CI + 1)) 
		|| CT == '-' && util::isNumber(this->getTokAtIndex(CI + 1))
		|| CT == '-' && this->getTokAtIndex(CI + 1) == '.' && util::isNumber(this->getTokAtIndex(CI + 2))) {
		string numExt(this->getNumExtends(CI));
		cout << numExt << endl;
		if (util::isFloat(&numExt)) {
			return lexer::scanResult{ createToken(tokenType::TT_float, numExt.c_str()), static_cast<unsigned int>(numExt.length())};
		}
		else if (util::isInt(&numExt)) {
			return lexer::scanResult{ createToken(tokenType::TT_int16, numExt.c_str()), static_cast<unsigned int>(numExt.length()) };
		}
	}

	return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, NULL), 1 };
}

string lexer::getNumExtends(unsigned int index) {
	string croppedSource(this->sourceFile.substr(index));
	string numExt;
	regex regExpr("[^\\w\\.]"); // split at each symbol apart from a dot

	// if number with minus, then cut it off becuase regex expr 
	// filters out syms, then add it back on later
	// otherwise just regex filter the text
	if (croppedSource.find_first_of("-") == 0) {
		string crop(croppedSource.substr(1));

		crop = util::strSplit(&crop, regExpr).at(0);
		crop.insert(0, "-");
		numExt = crop;
	}
	else {
		string cropBuffer(croppedSource);	
		cropBuffer = util::strSplit(&cropBuffer, regExpr).at(0);
		numExt = cropBuffer;
	}

	__int64 tally = ranges::count(numExt, '.');

	if (numExt.at(0) == '.')
		numExt.insert(0, "0");
	else if(numExt.at(0) == '-' && numExt.at(1) == '.')
		numExt.insert(1, "0");

	if (tally > 1) {
		mallformedInteger(&index, index + static_cast<int>(numExt.length()), "Float contains multiple decimals " + numExt, errorConfig{
			true,
			processorType::LUME_compiler,
		});
	}
	else if (!util::isNumber(numExt.c_str())) {
		if (util::isInt(&numExt))
			mallformedFloat(&index, index + static_cast<int>(numExt.length()), "Float contains invalid character " + numExt, errorConfig{
				true,
				processorType::LUME_compiler,
				});
		if (util::isFloat(&numExt))
			mallformedInteger(&index, index + static_cast<int>(numExt.length()), "integer contains invalid character " + numExt, errorConfig{
				true,
				processorType::LUME_compiler,
			});
	}

	string nextCharBuff(this->sourceFile.substr(index + numExt.length(), this->fileLength - (index + numExt.length())));
	string prevCharBuff(this->sourceFile.substr(index));
	this->condenseStr(&nextCharBuff);
	this->condenseStr(&prevCharBuff);
	char nextTok = nextCharBuff.at(1);
	char prevTok = prevCharBuff.at(max(static_cast<int>(prevCharBuff.length() - 1), 0));
	char prevSecTok = prevCharBuff.at(max(static_cast<int>(prevCharBuff.length() - 2), 0));
	if (prevCharBuff.length() - 2 < 0)
		prevSecTok = NULL;
	
	// check, because -2 ^ 2 is not allowed
	if (nextTok == '^' && croppedSource.at(0) == '-') {
		invalidOperation(&index, index + static_cast<int>(numExt.length()), "negative right side factor in unary operation", errorConfig{
			true,
			processorType::LUME_compiler,
			});
	}
	// checks if tokens allign correctly for an operation
	// It catches patterns like 1 1 or 1 ( 1
	// basically things that don't allign with 
	// standard operation terms
	else if (prevTok != NULL && util::isNumber(prevTok)
		|| prevTok != NULL && !lexer::isBinOp(prevTok) && (prevTok != '('
		|| prevTok == '(' && prevSecTok != NULL && prevSecTok != '(' && !lexer::isBinOp(prevSecTok))) {
		syntaxError(&index, index + static_cast<int>(numExt.length()), "Invalid operation. Expect binary operator, got " + prevTok, errorConfig{
			true,
			processorType::LUME_compiler,
		});
	}


	return numExt;
}