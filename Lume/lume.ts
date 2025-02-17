import { parseArgs  } from "jsr:@std/cli/parse-args";
import {existsSync} from "https://deno.land/std/fs/mod.ts";
import { compilerStatus, lumeCompiler } from "./src/compiler/compiler.ts";

let debugStatus : boolean;
let lumeVersion : string = "V1.00.1"

const flags = parseArgs(Deno.args, {
    boolean : ["debug"],
})

debugStatus = flags.debug;
if (flags._[0] === "build") {
    let directory : string;
    if (flags._[1]) {
        const filePath : string | number = flags._[1];
        if (typeof(filePath) !== "string") Deno.exit();
        if (!existsSync(filePath)) Deno.exit();
        directory = filePath;
    } else {
        directory = "./"
    }

    const compiler : lumeCompiler = new lumeCompiler(directory, {
        debug : debugStatus
    });
    const compilerStatus : compilerStatus = compiler.compile(directory);

    if (compilerStatus.status == true) {
        console.log("Compilation Succefull");
    } else {
        console.log("Compilation failed");
    }
}
