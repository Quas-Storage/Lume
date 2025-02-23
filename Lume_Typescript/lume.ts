import { parseArgs  } from "jsr:@std/cli/parse-args";
import {existsSync} from "https://deno.land/std/fs/mod.ts";
import { compilerStatus, lumeCompiler } from "./src/compiler/compiler.ts";

let lumeVersion : string = "V1.00.1"

const flags = parseArgs(Deno.args, {
    boolean : ["debug"],
})

// builds the selected file or program
function runProgram() : void {
    let directory : string;
    if (flags._[1]) {
        const filePath : string | number = flags._[1];
        if (typeof(filePath) !== "string") Deno.exit();
        if (!existsSync(filePath)) Deno.exit();
        directory = filePath;
    } else {
        directory = ".\\"
    }

    directory.replaceAll("/", "\\");
    const compiler : lumeCompiler = new lumeCompiler(directory, {
        debug : flags.debug,
    });
    let compilerStatus : compilerStatus;

    if (directory.endsWith(".lume")) {
        const fileSegments : string[] = directory.split("\\");
        const file : string | undefined = fileSegments.pop();
        const filePath : string = fileSegments.join("\\");

        if (file === undefined) return;
        const fileName : string[] = file.split(".")
        compilerStatus = compiler.compileFile(filePath, fileName[0]);
    } else {
        compilerStatus = compiler.compile();
    }

    if (compilerStatus.status == true) {
        console.log("Execution Succefull");
    } else {
        console.log("Execution failed");
    }
}

// displays the current lume version
function displayLumeVersion() : void {
    console.log("Currently running lume version " + lumeVersion);
}

// lume base
function dispalyLume() : void {
    console.log(" ")
    console.log("---------LUME---------");
    console.log("Version " + lumeVersion);
    console.log("                       ");
    console.log("use \"Help\" for more information");
    console.log(" ")
}

function displayHelp() : void {
    console.log(" ")
    console.log("\x1b[5mLume CLI guide\x1b[0m")
    console.log("help - provides instructions on the CLI");
    console.log("version - displays the installed version of Lume");
    console.log("build path_to_folder_or_file - builds a folder or file to .lmb files");
    console.log("   ")
    console.log("\x1b[5mflags\x1b[0m")
    console.log("build --debug - enables the debug mode for the compiler for better profiling");
}

if (flags._[0] === "build") {
    runProgram();
} else if(flags._[0] == "version") {
    displayLumeVersion();
} else if(flags._.length === 0){
    dispalyLume();
} else if(flags._[0] == "help") {
    displayHelp();
} else {
    console.log("unkown command")
}
