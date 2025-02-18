import { mallformedFloat, operationType } from "../lib/error.ts";
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
}

const binIdentifiers : string = "+-/*^%";
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

    public shiftCarrot(steps : number) {
        this.carrotIndex += steps;
    }

    public readIndex(carrotPosition : number) : [token | undefined, number] {
        const char : string | undefined = this.source.at(carrotPosition);
        if (char === undefined) throw new Error("Undefined error at carrot position " + carrotPosition)
        if (char === " ") return [undefined, 1]

        if (util.isDigit(char) || char === "." && util.isDigit(this.source.at(carrotPosition + 1))) {
            const numberExtends : string = this.getNumberExtends(carrotPosition);
            if (util.isFloat(numberExtends)) {
                return [{type : tokenType.float, value : numberExtends}, numberExtends.length];
            } else if(util.isInteger(numberExtends)) {
                return [{type : tokenType.int, value : numberExtends}, numberExtends.length];
            }
        }
        return [undefined, 1];
    }

    public tokenize() {
        while (this.carrotIndex < this.source.length && this.source.length > 0) {
            const tokenInformation : [token | undefined, number] = this.readIndex(this.carrotIndex);
            this.shiftCarrot(tokenInformation[1]);

            if (!tokenInformation[0]) continue;
            console.log(tokenInformation[0])
            this.tokens.push(tokenInformation[0])
        }
    }

    public getNumberExtends(carrotPosition : number) : string {
        const slicedCharacter : string = this.source.slice(carrotPosition, this.source.length)
        let numberSnippits : string = slicedCharacter.split(/[^\w\.]/)[0];
        const dotCount : null | string[] = numberSnippits.match(/\./g);

        if (numberSnippits[0] == ".") numberSnippits = "0" + numberSnippits;

        // numbers like 1.0.1
        if (dotCount !== null && dotCount.length >= 2) {
            new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Float contains decimals " + numberSnippits, {genStackTrace : true, operationType : operationType.compiler})
        // numbers like 1a
        } else if(!util.isDigit(numberSnippits)) {
            if (util.isInteger(numberSnippits)) new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Integer contains invalid character " + numberSnippits, {genStackTrace : true, operationType : operationType.compiler})
            if (util.isFloat(numberSnippits)) new mallformedFloat(carrotPosition, carrotPosition + numberSnippits.length, "Float contains invalid character " + numberSnippits, {genStackTrace : true, operationType : operationType.compiler})
        }

        return numberSnippits;
    }
}
