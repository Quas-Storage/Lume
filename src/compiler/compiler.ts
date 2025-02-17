// Specify folder which will be turend into a build
// read all the files in the build folder, and convert them to compiled files
// if no build folder specified, then create one
import {existsSync} from "https://deno.land/std/fs/mod.ts";
import { error } from "../lib/error.ts";
import { lexer, token } from "./lexer.ts";
import { parser } from "./parser.ts";

interface compilerStatus {
    status : boolean;
    errMessage : string | undefined;
}

enum dirType {
    folder,
    lumeFile,
}

type dirrBranch = Map<dirrBranch | string, dirType>
type dirrTree = Map<dirrBranch | string, dirType>

export class lumeCompiler {
    private directoryTree : dirrTree = new Map<dirrBranch | string, dirType>;

    constructor(folder : string) {
        // nestles throught the files and create a tree of the directories
        const readDir = (dirTree : string, dirTreeMap : dirrBranch) : void => {
            const directories : IteratorObject<Deno.DirEntry> = Deno.readDirSync(dirTree)
            for (const directoryEntry of directories) {
                if (directoryEntry.isDirectory) {
                    const subBranch : dirrBranch = new Map<dirrBranch | string, dirType>();
                    dirTreeMap.set(subBranch, dirType.folder);
                    readDir(dirTree + "/" + directoryEntry.name, subBranch);
                    continue;
                } else if (directoryEntry.isFile) {
                    if (!directoryEntry.name.endsWith(".lume")) continue;
                    dirTreeMap.set(dirTree + "/" + directoryEntry.name, dirType.lumeFile);
                    continue;
                }
            }
        }

        readDir(folder, this.directoryTree);
    }

    private compileLumeFile(lumeFileDirr : string) {
        const processFile = async () => {
            const decoder : TextDecoder = new TextDecoder("utf-8");
            const textData : BufferSource = await Deno.readFile(lumeFileDirr);
            const fileContent : string = decoder.decode(textData);

            error.currentLumeFile = fileContent;

            const lexerInstance : lexer = new lexer(fileContent);
            lexerInstance.tokenize();
            const tokenResult : token[] = lexerInstance.fetchTokens();

            const parserInstance : parser = new parser(tokenResult);
            parserInstance.parse();
        }
        processFile();
    }

    public compile(buildDirr : string) : compilerStatus {
        let errMessage : string | undefined = undefined;
        let status : boolean = true;

        try {
            const basePath = Deno.cwd() + buildDirr + "/build";
            if (existsSync(basePath)) throw new Error("Dir already exists");
            Deno.mkdirSync(basePath);

            const readBranch = (dirrBranch : dirrBranch) : void => {
                dirrBranch.forEach((v, k) => {
                    if (v === dirType.folder && typeof(k) !== "string") {
                        readBranch(k);
                        return;
                    } else if(v === dirType.lumeFile && typeof(k) === "string") {
                        this.compileLumeFile(k);
                        return;
                    }
                })
            }
            readBranch(this.directoryTree)
        } catch (err: unknown) {
            if (err instanceof Error) {
                errMessage = err.message
                status = false;
            }
        }

        return {
            status : status,
            errMessage : errMessage,
        }
    }
}

const sourceDirr : string | null = prompt("source Directory");
if (sourceDirr === null) throw new Error("Invalid Input Type");
if (!existsSync(sourceDirr)) throw new Error("Invalid Directory");

const buildDirr : string | null = prompt("build Directory");
if (buildDirr === null) throw new Error("Invalid Input Type");
if (!existsSync(buildDirr) && buildDirr !== "") throw new Error("Invalid Directory");

const compiler : lumeCompiler = new lumeCompiler(sourceDirr);
const compilerStatus : compilerStatus = compiler.compile(buildDirr)
if (!compilerStatus.status) {
    throw new Error("Exited compilation with error with message \n " + compilerStatus.errMessage);
} else {
    console.log("Compilation Succesfull");
}
