import { mallformedFloat, mallformedInteger, operationType } from "../lib/error.ts";
import { util } from "../util.ts";

////////////////////////////////////////////////////////////////////////////////////////////
// Tokens
////////////////////////////////////////////////////////////////////////////////////////////
export enum tokenType {
    integer,
    float,
    binOp,
    leftParen,
    rightParen,
};


export interface token {
    value : string;
    type : tokenType;
}

interface tokenExtend {
    content : string;
    length : number;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Patterns
////////////////////////////////////////////////////////////////////////////////////////////

const binIdentifiers : string = "+-/*^%";
const letterIdentifiers : string = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

export class lexer {
    private text : string;
    private tokenBuffer : token[];
    private carrotPosition : number;

    /**
     * Creates a new lexer instance to tokenize a given string
     * @param text - the text that will be tokenized
     * @param carrotPosition - the start position of the carrot
     */
    constructor(text : string, carrotPosition? : number) {
        this.text = text;
        this.tokenBuffer = [];
        this.carrotPosition = carrotPosition === undefined ? 0 : carrotPosition;
    }


    /**
     * Creates a tokes and returns it
     * @param value - the content of the token
     * @param tokenType - the type of token
     * @returns the token itself
     */
    private createToken(value : string, tokenType : tokenType) {
        const token : token = {
            value : value,
            type : tokenType,
        }
        return token;
    }

    /**
     * Figures the extends of a number out and returns the length of the token
     * @param carrotPosition - the position to start scanning at
     * @returns the content of the token and the carrot amount needed to skip to the next token
     */
    private getTokenExtends(carrotPosition : number) : tokenExtend {
        let result = "";
        let carrotShiftIndex : number = 0;

        const tokenExtends : tokenExtend = {
            content : result,
            length : carrotShiftIndex,
        }

        const baseShift : string | undefined = this.text.at(carrotPosition);
        if (baseShift === undefined) throw new Error("Carrot extend position " + carrotPosition + " Undefined")

        result += baseShift;
        tokenExtends.content = baseShift;
        carrotShiftIndex ++;
        tokenExtends.length ++;

        // Numbers (the second or is ment for number like .21 )
        if (Number(baseShift) || baseShift == "." && Number(this.text.at(carrotPosition + 1)) ) {
            let breakCondition : boolean = true;
            if (baseShift == ".") {
                result = 0 + result;
            }

            while (breakCondition) {
                const currentCharacter : string | undefined = this.text.at(carrotPosition + carrotShiftIndex);
                if (currentCharacter === undefined) break;

                if (currentCharacter === "." && util.isDigit(this.text.at(carrotPosition + carrotShiftIndex + 1)) || util.isDigit(currentCharacter)) {
                    result += currentCharacter;
                    carrotShiftIndex++;
                } else {
                    // gathers the remaining section for a complete error
                    if (letterIdentifiers.includes(currentCharacter)) {
                        let nextCharacter = this.text.at(carrotPosition + carrotShiftIndex);
                        while (true) {
                            if (nextCharacter === undefined || nextCharacter === "\n" || nextCharacter === " " || binIdentifiers.includes(nextCharacter) === true) break;
                            result += nextCharacter;
                            carrotShiftIndex++;
                            nextCharacter = this.text.at(carrotPosition + carrotShiftIndex);
                        }

                        if(util.isFloat(result)) new mallformedFloat(carrotPosition, carrotPosition + carrotShiftIndex, "Expected float, got " + result, {genStackTrace : true, operationType : operationType.compiler});
                        if(util.isInteger(result)) new mallformedInteger(carrotPosition, carrotPosition + carrotShiftIndex, "Expected integer, got " + result, {genStackTrace : true, operationType : operationType.compiler});
                    }
                    breakCondition = false;
                }
            }
            tokenExtends.content = result;
            tokenExtends.length = carrotShiftIndex;

            // while (Number(this.text.at(this.carrotPosition + carrotShiftIndex))) {
            //     result += this.text.at(this.carrotPosition + carrotShiftIndex) as string;
            //     carrotShiftIndex++;
            // }
            // tokenExtends.content = result;
            // tokenExtends.length = carrotShiftIndex;
        // Operators
        } else if(binIdentifiers.includes(baseShift) === true) {
            return tokenExtends;
        // left paranthese
        } else if(baseShift == "(") {
            return tokenExtends;
        // right paranthese
        } else if(baseShift == ")") {
            return tokenExtends;
        }

        return tokenExtends;
    }

    /**
     * Register and forms the current token at the carrots positions
     * @returns - Returns the token and carrot shift size needed to shift to the next expression
     */
    private registerIndex() : [token | undefined, number] {
        const shift : string | undefined = this.text.at(this.carrotPosition);

        // Filter out whitespace
        if (shift === " ") return [undefined, 1];
        if (shift === undefined) throw new Error("Carrot position " + this.carrotPosition + " Undefined")

        const tokenExtends : tokenExtend = this.getTokenExtends(this.carrotPosition);

        // Operators
        if (binIdentifiers.includes(tokenExtends.content) === true) {
            return [this.createToken(tokenExtends.content, tokenType.binOp), tokenExtends.length];
        // integers
        } else if(util.isDigit(tokenExtends.content) && !tokenExtends.content.includes(".")) {
            return [this.createToken(tokenExtends.content, tokenType.integer), tokenExtends.length];
        // Floats
        } else if(util.isDigit(tokenExtends.content) && tokenExtends.content.includes(".")) {
            return [this.createToken(tokenExtends.content, tokenType.float), tokenExtends.length];
        // left paranthese
        } else if(tokenExtends.content.includes("(")) {
            return [this.createToken(tokenExtends.content, tokenType.leftParen), tokenExtends.length];
        // right paranthese
        } else if(tokenExtends.content.includes(")")) {
            return [this.createToken(tokenExtends.content, tokenType.rightParen), tokenExtends.length];
        }

        return [undefined, 1];
    }

    /**
     * Moves the carrot to the next position
     * @param steps - the amount of steps the carrot should take
     */
    private shiftCarrot(steps : number) {
        this.carrotPosition += steps;
    }

    /**
     * Starts a new tokenizing process for the text given in the constructor
     */
    public tokenize() {
        let carrotLeap : number = 1;
        // Processes allt the token information
        const processToken = () => {
            const tokenInformation : [token | undefined, number] = this.registerIndex();
            carrotLeap = tokenInformation[1];

            if (tokenInformation[0] === undefined) return;
            this.tokenBuffer.push(tokenInformation[0])

            // console.log(tokenInformation[0], tokenInformation[1])
        }

        processToken(); // register the 0 index;
        while (this.carrotPosition < this.text.length - 1) {
            this.shiftCarrot(carrotLeap);
            processToken()
        }
    }

    /**
     * Fetches the tokenized version of the text
     * @returns the tokenized instance of the lexing process
     */
    public fetchTokens() : token[] {
        return this.tokenBuffer;
    }
}
