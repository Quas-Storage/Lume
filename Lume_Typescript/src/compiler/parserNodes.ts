import { tokenType, token } from "./lexer.ts";
import { parserBranch } from "./parser.ts";

export enum parserTypes {
    number,
    mul,
    div,
    add,
    sub,
    mod,
    pow,
    endl,
}

export class node {
    public TT : token;

    // converts token to string
    public toString() : string {
        return tokenType[this.TT.type] + ":" + this.TT.value.toString();
    }

    constructor(token : token) {
        this.TT = token;
    }
};

export class numberNode extends node {
    public type : parserTypes;
    public token : token;

    constructor(type : parserTypes, token : token) {
        super(token);
        this.type = type;
        this.token = token;
    }
}

export class binNode extends node {
    public type : parserTypes;
    public token : token;
    public left : numberNode | parserBranch | undefined; // undegined is for expresion like -(10 + 10)
    public right : numberNode | parserBranch;

    constructor(type : parserTypes, token : token, left : numberNode | parserBranch | undefined, right : numberNode | parserBranch) {
        super(token);
        this.type = type;
        this.left = left;
        this.right = right;
        this.token = token;
    }
}

export class endLineNode extends node {
    public type : parserTypes;
    public token : token;

    constructor(type : parserTypes, token : token) {
        super(token);
        this.type = type;
        this.token = token;
    }
}
