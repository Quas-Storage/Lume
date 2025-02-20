import { invalidOperation, mallformedFloat, operationType, syntaxError } from "../lib/error.ts";
import { util } from "../util.ts";

export enum tokenType {
    int,
    float,
    binOp,
    leftParen,
    rightParen,
};

export interface token {
    value : string;
    type : tokenType;
    pos : number;
}

export const binIdentifiers : string = "+-/*^%";
const letterIdentifiers : string = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

export class lexer {
    private source : string;
    private carrotIndex : number;
    private tokens : token[];

    constructor(source : string) {
        this.source = source;
        this.carrotIndex = 0;
        this.tokens = [];
    }

    // moves the carrot ahead
    public shiftCarrot(steps : number) {
        this.carrotIndex += steps;
    }

    // reads the letter at a certain carrot index
    public readIndex(carrotPosition : number) : [token | undefined, number] {
        const char : string | undefined = this.source.at(carrotPosition);
        if (char === undefined) throw new Error("Undefined error at carrot position " + carrotPosition)
        if (char === " ") return [undefined, 1]

        if (util.isDigit(char) || char === "." && util.isDigit(this.source.at(carrotPosition + 1)) || char === "-" && util.isDigit(this.source.at(carrotPosition + 1))) {
            const numberExtends : string = this.getNumberExtends(carrotPosition);
            if (util.isFloat(numberExtends)) {
                return [{type : tokenType.float, value : numberExtends, pos : carrotPosition}, numberExtends.length];
            } else if(util.isInteger(numberExtends)) {
                return [{type : tokenType.int, value : numberExtends, pos : carrotPosition}, numberExtends.length];
            }
        } else if(binIdentifiers.includes(char)) {
            return [{type : tokenType.binOp, value : this.checkBinSyntax(carrotPosition), pos : carrotPosition}, 1];
        } else if(char === "(") {
            return [{type : tokenType.leftParen, value : char, pos : carrotPosition}, 1];
        } else if(char === ")") {
            return [{type : tokenType.rightParen, value : char, pos : carrotPosition}, 1];
        }

        return [undefined, 1];
    }

    // starts the tokenizing process for the code
    public tokenize() {
        while (this.carrotIndex < this.source.length && this.source.length > 0) {
            const tokenInformation : [token | undefined, number] = this.readIndex(this.carrotIndex);
            this.shiftCarrot(tokenInformation[1]);

            if (!tokenInformation[0]) continue;
            console.log(tokenInformation[0])
            this.tokens.push(tokenInformation[0])
        }
    }

    // returns the lexed tokens
    public getTokens() : token[] {
        return this.tokens;
    }

    // fetches the next carrots to get the full number
    private getNumberExtends(carrotPosition : number) : string {
        const slicedCharacter : string = this.source.slice(carrotPosition, this.source.length)
        let numberSnippits : string;
        // makes it work with negative numbers
        if (slicedCharacter.indexOf("-") == 0) {
            numberSnippits = slicedCharacter.slice(1).split(/[^\w\.]/)[0]; // split at each symbol apart from a dot
            numberSnippits = "-" + numberSnippits;
        } else {
            numberSnippits = slicedCharacter.split(/[^\w\.]/)[0];
        }

        const dotCount : null | string[] = numberSnippits.match(/\./g);

        if (numberSnippits[0] == ".") numberSnippits = "0" + numberSnippits;
        // numbers like 1.0.1
        if (dotCount !== null && dotCount.length >= 2) {
            new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Float contains decimals " + numberSnippits, {
                genStackTrace : true,
                operationType : operationType.compiler
            })
        // numbers like 1a
        } else if(!util.isDigit(numberSnippits)) {
            if (util.isInteger(numberSnippits)) new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Integer contains invalid character " + numberSnippits, {
                genStackTrace : true,
                operationType : operationType.compiler
            })
            if (util.isFloat(numberSnippits)) new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Float contains invalid character " + numberSnippits, {
                genStackTrace : true,
                operationType : operationType.compiler
            })
        }

        const nextRightCharacter = this.source.slice(carrotPosition + numberSnippits.length, this.source.length).replaceAll(/[\s\n\r]/g, "") // replaces all space, \n and \r with space
        const nextleftCharacter = this.source.slice(0, carrotPosition).replaceAll(/[\s\n\r]/g, "")

        const nextToken : string | undefined = nextRightCharacter.at(1);
        const prevToken : string | undefined = nextleftCharacter.at(Math.max(nextleftCharacter.length - 1, 0));
        let prevPrevToken : string | undefined = nextleftCharacter.at(Math.max(nextleftCharacter.length - 2, 0));
        if (nextleftCharacter.length - 2 < 0) {
            prevPrevToken = undefined
        }

        // check, because -2 ^ 2 is not allowed
        if (nextToken === "^" && slicedCharacter.at(0) === "-") new invalidOperation(carrotPosition, carrotPosition + numberSnippits.length, "right side negative factor in unary operation", {
            genStackTrace : true,
            operationType : operationType.compiler
        })
        if (prevToken !== undefined && util.isDigit(prevToken) || prevToken !== undefined && !binIdentifiers.includes(prevToken) && (prevToken !== "(" || prevToken === "(" && prevPrevToken !== undefined && prevPrevToken !== "(" && !binIdentifiers.includes(prevPrevToken))) new syntaxError(carrotPosition, carrotPosition + numberSnippits.length, "Invalid unary operation. Expect binary operator, got " + prevToken, {
            genStackTrace : true,
            operationType : operationType.compiler
        })

        return numberSnippits;
    }

    // checks bin syntax for any errors
    // returns provided char for chaining
    private checkBinSyntax(carrotPosition : number) : string {
        const binOp : string | undefined = this.source.at(carrotPosition);
        if (!binOp) throw new Error("Carrot Position undefined at position " + carrotPosition);

        const nextRightCharacter = this.source.slice(carrotPosition, this.source.length).replaceAll(/[\s\n\r]/g, "") // replaces all space, \n and \r with space
        const nextleftCharacter = this.source.slice(0, carrotPosition).replaceAll(/[\s\n\r]/g, "")

        const nextToken : string | undefined = nextRightCharacter.at(1);
        const nextNextToken : string | undefined = nextRightCharacter.at(2);
        const prevToken : string | undefined = nextleftCharacter.at(Math.max(nextleftCharacter.length - 1, 0));

        if (nextToken !== undefined && nextToken === "(") return binOp;
        if (nextToken === undefined) new syntaxError(carrotPosition, carrotPosition + 1, "Right factor of binary operation empty", {
            genStackTrace : true,
            operationType : operationType.compiler
        })
        if (prevToken === undefined) new syntaxError(carrotPosition, carrotPosition + 1, "Left factor of binary operation empty", {
            genStackTrace : true,
            operationType : operationType.compiler
        })
        if (!util.isDigit(prevToken) && prevToken !== ")") new syntaxError(carrotPosition, carrotPosition + 1, "Left factor of binary operation invalid", {
            genStackTrace : true,
            operationType : operationType.compiler
        })
        if (!util.isDigit(nextToken) && nextToken === "-" && (nextNextToken === undefined || nextNextToken !== "(" && !util.isDigit(nextNextToken)) || !util.isDigit(nextToken) && nextToken !== "(" && nextToken !== "-") new syntaxError(carrotPosition, carrotPosition + 1, "Right factor of binary operation invalid", {
            genStackTrace : true,
            operationType : operationType.compiler
        })

        return binOp;
    }
}
