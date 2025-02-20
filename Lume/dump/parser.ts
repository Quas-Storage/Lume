// import { binIdentifiers, token, tokenType } from "./lexer.ts";

// export enum parserTypes {
//     number,
//     multiplication,
//     division,
//     addition,
//     subtraction,
// }

// enum associative {
//     left,
//     right,
// }

// interface operatorInformation {
//     precedence : number,
//     associative : associative,
// }

// const binOpMap : Map<string, operatorInformation> = new Map<string, operatorInformation>([
//     ["+", {precedence : 1, associative : associative.left}],
//     ["-", {precedence : 1, associative : associative.left}],
//     ["*", {precedence : 2, associative : associative.left}],
//     ["/", {precedence : 2, associative : associative.left}],
//     ["%", {precedence : 2, associative : associative.left}],
//     ["^", {precedence : 3, associative : associative.right}]
// ])

// export class parser {
//     private tokens : token[];
//     private currentToken : token;
//     private currentCarrotPosition : number;

//     constructor(tokens : token[]) {
//         this.tokens = tokens;
//         this.currentCarrotPosition = 0;
//         this.currentToken = this.tokens[0];
//     }

//     private shiftCarrot(steps : number) {
//         if (this.currentCarrotPosition + steps >= this.tokens.length) return;
//         this.currentCarrotPosition += steps;
//         this.currentToken = this.tokens[this.currentCarrotPosition];
//     }

//     public parse() {
//         if (this.tokens.length === 0) return;
//         const result : number = this.computeExpression(1);
//         console.log(result);
//     }

//     private computeAtom() : number {
//         const currentToken : token = this.currentToken;

//         if (currentToken === undefined) {
//             throw new Error("Current Token is undefined at position" + this.currentCarrotPosition);
//         } else if (currentToken.type === tokenType.leftParen) {
//             this.shiftCarrot(1);
//             const leftAtom : number = this.computeExpression(1);
//             // check if paranthesiedm expression is empty
//             if (this.currentToken.type !== tokenType.rightParen) {
//                 throw new Error("Right parenthese expected, got " + this.currentToken.type);
//             }
//             this.shiftCarrot(1);

//             return leftAtom;
//         } else if (currentToken.type === tokenType.binOp) {
//             throw new Error("bin op");
//         } else {
//             this.shiftCarrot(1);
//             return Number(currentToken.value);
//         }
//     }

//     private computeOperator(operator : string, leftAtom : number, rightAtom : number) : number {
//         console.log(operator, leftAtom, rightAtom)
//         switch (operator) {
//             case "+": return leftAtom + rightAtom;
//             case "-": return leftAtom - rightAtom;
//             case "*": return leftAtom * rightAtom;
//             case "/": return leftAtom / rightAtom;
//             case "^": return leftAtom ** rightAtom;
//             case "%": return leftAtom % rightAtom;
//             default: throw new Error("Unkown Operator");
//         }
//     }

//     private computeExpression(minimumPrecedence : number) : number {
//         let leftAtom : number = this.computeAtom();
//         while (this.currentToken.type === tokenType.binOp && (binOpMap.get(this.currentToken.value) as operatorInformation).precedence >= minimumPrecedence) {
//             const currentToken = this.currentToken;
//             const opInformation : operatorInformation = binOpMap.get(this.currentToken.value) as operatorInformation;

//             let nextMinPrecedence : number = opInformation.associative === associative.left ? opInformation.precedence + 1 : opInformation.precedence;

//             this.shiftCarrot(1);
//             const rightAtom : number = this.computeExpression(nextMinPrecedence);
//             leftAtom = this.computeOperator(currentToken.value, leftAtom, rightAtom);
//         }

//         return leftAtom;
//     }
// }
