import { lexer, token } from "./lexer.ts"
import { parser } from "./parser.ts";

export class program {
    public static execute(code : string) : void {
        const lexerInstance : lexer = new lexer(code);
        lexerInstance.tokenize();

        const tokenResult : token[] = lexerInstance.fetchTokens();

        const parserInstance : parser = new parser(tokenResult);
        parserInstance.parse();
    }
}
