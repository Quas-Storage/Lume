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

token lexer::createToken(tokenType type, string value) {
	token tok = token{
		value,
		type,
		this->caretIndex
	};
	return tok;
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
		return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, "NULL"), 1};

	if (util::isNumber(CT) 
		|| CT == '.' && util::isNumber(this->getTokAtIndex(CI + 1)) 
		|| CT == '-' && util::isNumber(this->getTokAtIndex(CI + 1))
		|| CT == '-' && this->getTokAtIndex(CI + 1) == '.' && util::isNumber(this->getTokAtIndex(CI + 2))) {
		string numExt(this->getNumExtends(CI));
		if (util::isFloat(&numExt)) {
			return lexer::scanResult{ createToken(tokenType::TT_float, numExt), static_cast<unsigned int>(numExt.length())};
		}
		else if (util::isInt(&numExt)) {
			return lexer::scanResult{ createToken(tokenType::TT_int16, numExt), static_cast<unsigned int>(numExt.length()) };
		}
	}
	else if (lexerHelper::isBinOp(CT)) {
		return lexer::scanResult{ createToken(tokenType::TT_binOp, lexer::checkBinSyntax(CI)), 1};
	}
	else if (CT == '(') {
		return lexer::scanResult{ createToken(tokenType::TT_leftParen, string(1,CT)), 1 };
	}
	else if (CT == ')') {
		return lexer::scanResult{ createToken(tokenType::TT_rightParen, string(1,CT)), 1 };
	}

	return lexer::scanResult{ createToken(tokenType::NULL_TOKEN, "NULL"), 1};
}

string lexer::getNumExtends(unsigned int index) {
	string croppedSource(this->sourceFile.substr(index));
	string numExt;

	// if number with minus, then cut it off becuase regex expr 
	// filters out syms, then add it back on later
	// otherwise just regex filter the text
	if (croppedSource.find_first_of("-") == 0) {
		string crop(croppedSource.substr(1));
		crop = lexerHelper::splitStrOnSym(&crop).at(0);
		crop.insert(0, "-");
		numExt = crop;
	}
	else {
		string cropBuffer(croppedSource);
		cropBuffer = lexerHelper::splitStrOnSym(&cropBuffer).at(0);
		numExt = cropBuffer;
	}

	__int64 tally = ranges::count(numExt, '.');

	if (numExt[0] == '.')
		numExt.insert(0, "0");
	else if (numExt[0] == '-'  && numExt[1] && numExt[1] == '.')
		numExt.insert(1, "0");
	
	if (tally > 1) {
		mallformedInteger(&index, index + static_cast<int>(numExt.length()), "Float contains multiple decimals '" + numExt + "'", errorConfig{
			true,
			processorType::LUME_compiler,
		});
	}
	else if (!util::isNumber(numExt.c_str())) {
		if (util::isInt(&numExt))
			mallformedFloat(&index, index + static_cast<int>(numExt.length()), "Float contains invalid character '" + numExt + "'", errorConfig{
				true,
				processorType::LUME_compiler,
				});
		if (util::isFloat(&numExt))
			mallformedInteger(&index, index + static_cast<int>(numExt.length()), "integer contains invalid character '" + numExt + "'", errorConfig{
				true,
				processorType::LUME_compiler,
			});
	}


	bool prevIsEmpty = false;
	string nextCharBuff(this->sourceFile.substr(index + numExt.length(), this->fileLength - (index + numExt.length())));
	string prevCharBuff;
	if (index == 0) {
		prevCharBuff = this->sourceFile;
		prevIsEmpty = true;
	}
	else {
		prevCharBuff = this->sourceFile.substr(0, index);
	}

	lexerHelper::condenseStr(&nextCharBuff);
	lexerHelper::condenseStr(&prevCharBuff);
	char nextTok = lexerHelper::atIndexIsValid(&nextCharBuff, 0)
		? nextCharBuff[0]
		: ' ';
	char prevTok = lexerHelper::atIndexIsValid(&prevCharBuff, max(static_cast<int>(prevCharBuff.length() - 1), 0))
		? prevCharBuff[max(static_cast<int>(prevCharBuff.length() - 1), 0)]
		: ' ';
	char prevSecTok = lexerHelper::atIndexIsValid(&prevCharBuff, max(static_cast<int>(prevCharBuff.length() - 2), 0))
		? prevCharBuff[max(static_cast<int>(prevCharBuff.length() - 2), 0)]
		: ' ';

	// check, because -2 ^ 2 is not allowed
	if (nextTok == '^' && croppedSource[0] == '-') {
		invalidOperation(&index, index + static_cast<int>(numExt.length()), "negative right side factor in unary operation", errorConfig{
			true,
			processorType::LUME_compiler,
			});
	}
	else if (prevCharBuff.length() - 2 < 0 || prevIsEmpty)
		return numExt;
	// checks if tokens allign correctly for an operation
	// It catches patterns like 1 1 or 1 ( 1
	// basically things that don't allign with 
	// standard operation terms
	else if (util::isNumber(prevTok)
		|| !lexerHelper::isBinOp(prevTok) && (prevTok != '('
		|| prevTok == '(' && prevSecTok != NULL && prevSecTok != '(' && !lexerHelper::isBinOp(prevSecTok))) {
		string str(1, prevTok);
		syntaxError(&index, index + static_cast<int>(numExt.length()), "Invalid operation. Expect binary operator, got '" + str + "'", errorConfig{
			true,
			processorType::LUME_compiler,
		});
	}

	return numExt;
}

string lexer::checkBinSyntax(unsigned int index) {
	if (!currentToken)
		throw runtime_error("Carrot Position undefined at position " + index);

	bool prevIsEmpty = false;
	string nextCharBuff(this->sourceFile.substr(index + 1, this->fileLength - (index + 1))); // plus once for the current char
	string prevCharBuff;

	if (index == 0) {
		prevCharBuff = this->sourceFile;
		prevIsEmpty = true;
	}
	else {
		prevCharBuff = this->sourceFile.substr(0, index);
	}

	lexerHelper::condenseStr(&nextCharBuff);
	lexerHelper::condenseStr(&prevCharBuff);
	int prevIndex = max(static_cast<int>(prevCharBuff.length() - 1), 0);
	char nextTok =	lexerHelper::atIndexIsValid(&nextCharBuff, 0) 
		? nextCharBuff[0]
		: ' ';
	char nextSecTok = lexerHelper::atIndexIsValid(&nextCharBuff, 1)
		? nextCharBuff[1]
		: ' ';
	char prevTok = lexerHelper::atIndexIsValid(&prevCharBuff, prevIndex)
		? prevCharBuff[prevIndex]
		: ' ';

	if (nextTok != ' ' && nextTok == '(')
		return string(1, currentToken);
	else if (nextTok == ' ')
		syntaxError(&index, index + 1, "Right factor of binary operation empty", errorConfig{
			true,
			processorType::LUME_compiler,
		});
	else if(prevTok == ' ') 
		syntaxError(&index, index + 1, "left factor of binary operation empty", errorConfig{
			true,
			processorType::LUME_compiler,
		});
	else if(!util::isNumber(prevTok) && prevTok != ')')
		syntaxError(&index, index + 1, "left factor of binary operation invalid", errorConfig{
			true,
			processorType::LUME_compiler,
		});
	else if(!util::isNumber(nextTok) && nextTok == '-' && (nextSecTok == ' '
		|| nextSecTok != '(' && !util::isNumber(nextSecTok))
		|| !util::isNumber(nextTok) && nextTok != '(' && nextTok != '-')
		syntaxError(&index, index + 1, "Right factor of binary operation invalid", errorConfig{
			true,
			processorType::LUME_compiler,
		});

	return string(1, currentToken);
}


////////////////////////////////////////////////////////
// Lexer helper funcs
////////////////////////////////////////////////////////


// checks if index provided fits within a string
bool lexerHelper::atIndexIsValid(string* source, unsigned int index) {
	return index >= static_cast<unsigned int>(source->length()) 
		? false 
		: true;
}

bool lexerHelper::isBinOp(char binOp) {
	string BID = BIN_IDENTIFIERS;
	return BID.find(binOp) != string::npos;
}

void lexerHelper::condenseStr(string* str) {
	str->erase(remove_if(str->begin(), str->end(), [](unsigned char _C) {
		return isspace(_C) || _C == '\n' || _C == '\r';
		}), str->end());
}

// splits string on every symbol and whitespace apart from dot
vector<string> lexerHelper::splitStrOnSym(string* str) {
	vector<string> res;
	string temp;
	for (const char _C : *str) {
		if (ispunct(_C) && _C != '.' || isspace(_C)) {
			res.push_back(temp);
			temp = "";
		}
		else {
			temp += string(1, _C);
		}
	}
	return res;
}