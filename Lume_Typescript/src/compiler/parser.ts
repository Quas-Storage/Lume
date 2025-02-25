import { operationType, syntaxError } from "../lib/error.ts";
import { token, tokenType } from "./lexer.ts";
import { binNode, endLineNode, node, numberNode, parserTypes } from "./parserNodes.ts";

enum associative {
    left,
    right,
}

interface operatorInformation {
    precedence : number,
    associative : associative,
}

interface parserToken extends token {
    read : boolean;
}

const binOpMap : Map<string, operatorInformation> = new Map<string, operatorInformation>([
    ["+", {precedence : 1, associative : associative.left}],
    ["-", {precedence : 1, associative : associative.left}],
    ["*", {precedence : 2, associative : associative.left}],
    ["/", {precedence : 2, associative : associative.left}],
    ["%", {precedence : 2, associative : associative.left}],
    ["^", {precedence : 3, associative : associative.right}]
])

export type parserTree = Array<node | parserBranch>;
export type parserBranch = Array<node | parserBranch>;

export class parser {
    private tokens : parserToken[] = [];
    private currentToken : parserToken;
    private currentCarrotPosition : number;
    private parserTree : parserTree;

    constructor(tokens : token[]) {
        tokens.map(Token => this.tokens.push({
            value : Token.value,
            type : Token.type,
            pos : Token.pos,
            read : false,
        }))

        this.currentCarrotPosition = 0;
        this.currentToken = this.tokens[0];
        this.parserTree = new Array<node>
    }

    // moves the parser carrot to the next step
    private shiftCarrot(steps : number) {
        if (this.currentCarrotPosition + steps >= this.tokens.length) return;
        this.currentCarrotPosition += steps;
        this.currentToken = this.tokens[this.currentCarrotPosition];
    }

    // Does the magic ig...
    public parse() {
        if (this.tokens.length === 0) return;
        this.parserTree = this.parseLine(new Array<node>);
        this.displayParseTree();
    }

    // fyi, it displays [array] when file's to large
    public displayParseTree() : void {
        const readBranch = (Branch : parserTree | parserBranch, recBranch : Array<unknown>) : Array<unknown> => {
            if (Branch === undefined) return recBranch;
            Branch.forEach(nodeInstance => {
                if (nodeInstance instanceof node) {
                    recBranch.push(nodeInstance.toString());
                } else {
                    recBranch.push(readBranch(nodeInstance, new Array<unknown>))
                    return recBranch
                }
            })

            return recBranch;
        }
        const reconstuctedTree = readBranch(this.parserTree, new Array<unknown>);
        console.log(reconstuctedTree);
    }

    // Reads a number (factor) in an unary operation
    private computeAtom(branch : parserBranch) : numberNode | parserBranch {
        const currentToken : parserToken = this.currentToken;
        console.log(currentToken);
        if (currentToken === undefined) {
            throw new Error("Current Token is undefined at position" + this.currentCarrotPosition);
        } else if (currentToken.type === tokenType.leftParen) {
            this.shiftCarrot(1);
            currentToken.read = true;
            const leftAtom : parserBranch = this.computeExpression(1, new Array<node>)

            if (this.currentToken.type !== tokenType.rightParen || this.currentToken.type === tokenType.rightParen && this.currentToken.read) {
                new syntaxError(currentToken.pos, this.currentToken.pos + 2, "Parathesized expression not closed. Expected ')'", {
                    genStackTrace : true,
                    operationType : operationType.compiler
                })
            }
            this.currentToken.read = true;
            this.shiftCarrot(1);
            branch.push(leftAtom);

            return leftAtom;
        } else if (currentToken.type === tokenType.binOp) {
            // for expression that arent negatively paranthesized (aka -(10 + 10) )
            if (currentToken.value === "-") {
                currentToken.read = true;
                this.shiftCarrot(1);
                const rightAtom : numberNode | parserBranch = this.computeAtom(new Array<node>)
                const binNode : binNode = this.computeOperator(currentToken, undefined, rightAtom)

                branch.push(binNode);
                branch.push(rightAtom);
                return rightAtom
            }

            new syntaxError(currentToken.pos, currentToken.pos + 1, "Unexpected binary operator '" + currentToken.value +"' in expression. Expected a valid operator like '+', '-', '*', or '/'. ", {
                genStackTrace : true,
                operationType : operationType.compiler,
            })
            throw new Error("bin op"); // unreached, but will type error when removed
        } else {
            currentToken.read = true;
            this.shiftCarrot(1);
            const node : numberNode = new numberNode(parserTypes.number, currentToken);
            branch.push(node);
            return node
        }
    }

    // creates binary nodes
    private computeOperator(operator : token, leftAtom : numberNode | parserBranch | undefined, rightAtom : numberNode | parserBranch) : binNode {
        switch (operator.value) {
            case "+": return new binNode(parserTypes.add, operator, leftAtom, rightAtom);
            case "-": return new binNode(parserTypes.sub, operator, leftAtom, rightAtom);
            case "*": return new binNode(parserTypes.mul, operator, leftAtom, rightAtom);
            case "/": return new binNode(parserTypes.div, operator, leftAtom, rightAtom);
            case "^": return new binNode(parserTypes.pow, operator, leftAtom, rightAtom);
            case "%": return new binNode(parserTypes.mod, operator, leftAtom, rightAtom);
            default: throw new Error("Unkown Operator");
        }
    }

    // computes mathmatical expresssions
    private computeExpression(minimumPrecedence : number, branch : parserBranch) : parserBranch {
        const leftAtom : numberNode | parserBranch = this.computeAtom(branch);

        while (this.currentToken.type === tokenType.binOp && (binOpMap.get(this.currentToken.value) as operatorInformation).precedence >= minimumPrecedence) {
            const currentToken : token = this.currentToken;
            const opInformation : operatorInformation = binOpMap.get(this.currentToken.value) as operatorInformation;

            const nextMinPrecedence : number = opInformation.associative === associative.left ? opInformation.precedence + 1 : opInformation.precedence;

            this.shiftCarrot(1);
            const rightSide : parserBranch = this.computeExpression(nextMinPrecedence, new Array<node>);
            const binOp = this.computeOperator(currentToken, leftAtom, rightSide);

            branch.push(binOp);
            branch.push(rightSide.length > 1 ? rightSide : rightSide[0]);
        }

        return branch;
    }

    private parseEOL(branch : parserBranch, token : parserToken) {
        const endLineToken : node = new endLineNode(parserTypes.endl, token);
        branch.push(endLineToken);
        const subBranch : parserBranch = this.parseLine(new Array<node>);
        if (subBranch.length !== 0) {
            branch.push(subBranch);
        }
    }

    // reads a line and constructs the parser tree
    private parseLine(branch : parserBranch) {
        const currentCarrotPosition : number = this.currentCarrotPosition;
        let currentToken : parserToken | undefined = this.currentToken;

        while (currentToken !== undefined && currentToken.type !== tokenType.EOL) {
            if (currentToken.type === tokenType.binOp || currentToken.type === tokenType.int || currentToken.type === tokenType.float) {
                const expressionBranch : parserBranch = this.computeExpression(1, new Array<node>);
                this.shiftCarrot(-1);
                branch.push(expressionBranch);
            }
            this.shiftCarrot(1);
            currentToken = this.currentToken;

            if (currentToken.type === tokenType.EOL) {
                this.parseEOL(branch, currentToken);
            } else if(currentToken.type === tokenType.rightParen && !currentToken.read) {
                new syntaxError(this.currentToken.pos, this.currentToken.pos + 1, "closing parenthesis ')' does not match opening parenthesis '('", {
                    genStackTrace : true,
                    operationType : operationType.compiler,
                })
            }
        }

        return branch;
    }
}
