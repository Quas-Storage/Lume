import { token } from "../../dump/lexer.ts";
import { parserTypes } from "./parser.ts";

export class numberNode {
    public type : parserTypes;
    public token : token;

    constructor(type : parserTypes, token : token) {
        this.type = type;
        this.token = token;
    }
}

export class binNode {
    public type : parserTypes;
    public token : token;
    public left : numberNode;
    public right : numberNode;

    constructor(type : parserTypes, token : token, left : numberNode, right : numberNode) {
        this.type = type;
        this.left = left;
        this.right = right;
        this.token = token;
    }
}
