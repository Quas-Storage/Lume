export class util {
    public static isDigit(string : string | undefined) : boolean {
        // /^[+-]?\d+(\.\d+)?$/
        if (typeof(string) !== "string") throw new Error("Expected string, got " + typeof(string));
        return /^[+-]?\d+(\.\d+)?([eE][+-]?\d+)?$/.test(string);
    }

    public static isFloat(number : string | number) {
        return number.toString().includes(".")
    }

    public static isInteger(number : string | n umber) {
        return !number.toString().includes(".")
    }

    public static clamp(a : number, b : number, c : number) {
        return Math.min(Math.max(a, b), c);
    }
}
