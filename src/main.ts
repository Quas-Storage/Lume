import { program } from "./program.ts"

const file : string = "program.lume"

if (!file.endsWith(".lume")) throw new Error("Invalid File Type. Expected .Lume");

const decoder : TextDecoder = new TextDecoder("utf-8");
const textData : BufferSource = await Deno.readFile(file);
const fileContent : string = decoder.decode(textData);

program.execute(fileContent);
