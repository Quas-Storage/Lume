enum errorType {
    undefinedCharactrer,
    mallformedInteger,
    mallformedFloat,
}

interface errorConfig {
    genStackTrace : boolean;
}


export class error {
    private posStart : number;
    private posEnd : number;
    private errorCode : errorType;
    private details : string;
    private errorConfig : errorConfig;

    public static currentLumeFile : string; // the current file being processed

    private throw() {
        if (error.currentLumeFile === undefined) throw new Error("Lume file not specified");

        // we split each line into an array, than add the length of the line
        // if the length of the line surpasses the startpos, we know the error is in that line
        const programLines : string[] = error.currentLumeFile.split("\n");
        let currentCarrotPosition : number = 0;
        let lastCarrotPosition : number = 0;
        let lineNumber : number = 0;
        for (lineNumber; lineNumber < programLines.length; lineNumber++) {
            currentCarrotPosition += Math.abs(programLines[lineNumber].length + 1) //  we add plus one beacuse the endline is removed which also counts as a valid character. Why? hell tf do i know?
            if (this.posStart <= currentCarrotPosition) {
                lineNumber++; // fix the offset cuz editors start at line one, not zero
                break;
            }
            lastCarrotPosition = currentCarrotPosition;
        }

        const beginCode : string = error.currentLumeFile.slice(lastCarrotPosition, this.posStart);
        const middleCode : string = error.currentLumeFile.slice(this.posStart, this.posEnd);
        const endCode : string = error.currentLumeFile.slice(this.posEnd, currentCarrotPosition - 1);

        let ErrorMessage ="\x1b[91mERROR; Line:" + lineNumber.toString() + " ";
        ErrorMessage += "Code:" + this.errorCode.toString() + " "
        ErrorMessage += errorType[this.errorCode] + "\n";
        ErrorMessage += beginCode + middleCode + endCode + "\n"
        ErrorMessage += beginCode.replace(/[\x20-\x7E]/g, " ") + middleCode.replace(/[\x20-\x7E]/g, "^") + "\n"
        ErrorMessage += this.details + "\x1b[0m";
        console.log("--------------------------------------------------------------------------------------------------------------")
        console.log(ErrorMessage);
        console.log("--------------------------------------------------------------------------------------------------------------")

        if (this.errorConfig.genStackTrace) {
            const stackTrace : string[] = this.genStackTrace();
            // Have to actually wait for multiple files
        }
    }

    constructor(posStart : number, posEnd : number, errorCode : errorType, details : string, config : errorConfig) {
        this.posEnd = posEnd;
        this.posStart = posStart;
        this.errorCode = errorCode;
        this.details = details;
        this.errorConfig = config;

        this.throw();
    }

    public genStackTrace() : string[] {
        // still todo
        return []
    }
}

// mallformed integer like 1a345
export class mallformedInteger extends error {
    constructor(posStart : number, posEnd : number, details : string, config : errorConfig ) {
        super(posStart, posEnd, errorType.mallformedInteger, details, config);
    }
}
