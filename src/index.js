const compiler = require("./compiler/compiler");
const fs = require("fs");

(async () => {
    const args = process.argv
    const src = (await fs.readFileSync(args[2])).toString().replace(/[\n\r]+/g, ' ')
    console.log(src)
    console.log(await compiler.compile(src))
})();