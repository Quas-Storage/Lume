import { operationType, syntaxError } from "../lib/error.ts";
import { binIdentifiers, token, tokenType } from "./lexer.ts";
import { binNode, node, numberNode, parserTypes } from "./parserNodes.ts";

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

    private shiftCarrot(steps : number) {
        if (this.currentCarrotPosition + steps >= this.tokens.length) return;
        this.currentCarrotPosition += steps;
        this.currentToken = this.tokens[this.currentCarrotPosition];
    }

    public parse() {
        if (this.tokens.length === 0) return;
        this.parserTree = this.computeExpression(1, new Array<node>);
        this.displayParseTree();
    }

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

    private computeAtom(branch : parserBranch) : numberNode | parserBranch {
        const currentToken : parserToken = this.currentToken;

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
            throw new Error("bin op");
        } else {
            currentToken.read = true;
            this.shiftCarrot(1);
            const node : numberNode = new numberNode(parserTypes.number, currentToken);
            branch.push(node);
            return node
        }
    }

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
}
