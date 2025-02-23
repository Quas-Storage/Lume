import { error } from "../lib/error.ts";
import { lexer, token } from "./lexer.ts";
import {existsSync} from "https://deno.land/std/fs/mod.ts";
import { parser } from "./parser.ts";

export interface compilerStatus {
    status : boolean;
    errMessage : string | undefined;
}

interface compilerOptions {
    debug : boolean;
}

enum dirType {
    folder,
    lumeFile,
}

type dirrBranch = Map<dirrBranch | string, [string, dirType]>
type dirrTree = Map<dirrBranch | string, [string, dirType]>

export class lumeCompiler {
    private directoryTree : dirrTree = new Map<dirrBranch | string, [string, dirType]>;
    private compilerOptions : compilerOptions;
    private lumeFile : string = "";

    // creates the file tree for the compiler to deconstruct and rebuild in the build folder
    constructor(folder : string, options : compilerOptions) {
        // nestles throught the files and create a tree of the directories
        const readDir = (dirTree : string, dirTreeMap : dirrBranch) : void => {
            const directories : IteratorObject<Deno.DirEntry> = Deno.readDirSync(dirTree)
            for (const directoryEntry of directories) {
                if (directoryEntry.isDirectory) {
                    if (directoryEntry.name === "build") return;
                    const subBranch : dirrBranch = new Map<dirrBranch | string, [string, dirType]>();
                    dirTreeMap.set(subBranch, [directoryEntry.name, dirType.folder]);
                    readDir(dirTree + "\\" + directoryEntry.name, subBranch);
                    continue;
                } else if (directoryEntry.isFile) {
                    if (!directoryEntry.name.endsWith(".lume")) continue;
                    dirTreeMap.set(dirTree + "\\" + directoryEntry.name, [directoryEntry.name, dirType.lumeFile]);
                    continue;
                }
            }
        }

        if (folder.endsWith(".lume")) {
            this.lumeFile = folder;
        } else {
            readDir(folder, this.directoryTree);
        }
        this.compilerOptions = options;
    }

    // compiles a singular lume file
    private compileLumeFile(lumeFileDirr : string) {
        const decoder : TextDecoder = new TextDecoder("utf-8");
        const textData : BufferSource = Deno.readFileSync(lumeFileDirr);
        const fileContent : string = decoder.decode(textData);

        error.currentLumeFile = fileContent;

        console.log("Compiling: " + (lumeFileDirr.startsWith(".\\") || lumeFileDirr.startsWith("./") ? lumeFileDirr.substring(2) : lumeFileDirr));

        const lexerInstance : lexer = new lexer(fileContent);
        lexerInstance.tokenize();
        const tokenResult : token[] = lexerInstance.getTokens();

        const parserInstance : parser = new parser(tokenResult);
        parserInstance.parse();
    }

    // compiles single lume file
    public compileFile(fileDir : string, fileName : string) {
        let errMessage : string | undefined = undefined;
        let status : boolean = true;
        if (!existsSync(fileDir)) throw new Error("directory unkown " + fileDir);

        try {
            this.compileLumeFile(this.lumeFile);

            const encode : TextEncoder = new TextEncoder()
            const encodedTextData : Uint8Array = encode.encode("test")
            Deno.writeFileSync(fileDir + "\\" + fileName + ".lmb", encodedTextData);
        } catch(err : unknown) {
            if (err instanceof Error && this.compilerOptions.debug) {
                errMessage = err.message;
                status = false;
            }
        }

        if (this.compilerOptions.debug && !status) {
            throw new Error("Exited compilation with error with message \n " + errMessage);
        }

        return {
            status : status,
            errMessage : errMessage,
        }
    }

    // compiles the linked folder
    public compile() : compilerStatus {
        let errMessage : string | undefined = undefined;
        let status : boolean = true;

        try {
            const readBranch = (dirrBranch : dirrBranch) : void => {
                dirrBranch.forEach((v, k) => {
                    if (v[1] === dirType.folder && typeof(k) !== "string") {
                        readBranch(k);
                        return;
                    } else if(v[1] === dirType.lumeFile && typeof(k) === "string") {
                        this.compileLumeFile(k);
                        return;
                    }
                })
            }
            readBranch(this.directoryTree)
        } catch (err: unknown) {
            if (err instanceof Error && this.compilerOptions.debug) {
                errMessage = err.message;
                status = false;
            }
        }

        if (this.compilerOptions.debug && !status) {
            throw new Error("Exited compilation with error with message \n " + errMessage);
        }

        return {
            status : status,
            errMessage : errMessage,
        }
    }
}





// const sourceDirr : string | null = prompt("source Directory");
// if (sourceDirr === null) throw new Error("Invalid Input Type");
// if (!existsSync(sourceDirr)) throw new Error("Invalid Directory");

// const buildDirr : string | null = prompt("build Directory");
// if (buildDirr === null) throw new Error("Invalid Input Type");
// if (!existsSync(buildDirr) && buildDirr !== "") throw new Error("Invalid Directory");

// const compiler : lumeCompiler = new lumeCompiler(sourceDirr);
// const compilerStatus : compilerStatus = compiler.compile(buildDirr)

// if (!compilerStatus.status) {
//     throw new Error("Exited compilation with error with message \n " + compilerStatus.errMessage);
// } else {
//     console.log("Compilation Succesfull");
// }
