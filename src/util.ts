export class util {
    public static isDigit(string : string | undefined) : boolean {
        if (typeof(string) !== "string") throw new Error("Expected string, got " + typeof(string));
        return /^[+-]?\d+(\.\d+)?$/.test(string);
    }
}
