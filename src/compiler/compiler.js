const lexer = require('./lexer')

module.exports = {
    
    compile: async (src) => {
        var tokens = lexer.lex(src)
    }
}