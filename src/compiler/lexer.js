const alpha = /[a-zA-Z]/i
const num   = /[0-9]/

const TokenType = {
    tok_eof: -1,

    tok_keyword: -2,
    tok_identifier: -3,
    tok_operator: -4,
    
    tok_string: -5,
    tok_number: -6,
    tok_float: -7,

    tok_brackets: -8,
    tok_parenthesis: -9,
    tok_arrow: -10
}

module.exports = {
    lex: async (src) => {
        var iteration = 0
        var current = ''
        var identifierString = ''
        var numberString = ''
        const getNextChar = () => {
            return current = src[iteration++];
        }

        const getNextToken = () => {
            getNextChar()

            while (current == ' ' || current == '\n')
                getNextChar()

            if (alpha.test(current)) {
                identifierString = current
                getNextChar()
                while (alpha.test(current) || num.test(current)) {
                    if (current == null)
                        return null
                    identifierString += current
                    getNextChar()
                }
                
                if (identifierString === 'fn')
                    return TokenType.tok_keyword

                return TokenType.tok_identifier
            }

            if (num.test(current) || current === '.') {
                numberString = current
                getNextChar()
                while (num.test(current) || current === '.') {
                    numberString += current
                    getNextChar()
                }

                return (numberString.includes('.') ? TokenType.tok_float : TokenType.tok_number)
            }

            if (current == '{' || current == '}')
                return TokenType.tok_brackets

            if (current == '(' || current == ')')
                return TokenType.tok_parenthesis

            if (current == '<' || current == '>')
                return TokenType.tok_arrow

            if (current == '+' || current == '-' || current == '*' || current == '/')
                return TokenType.tok_operator
        }

        const tokenList = []
        var token = getNextToken()
        while (token != null) {
            console.log(tokenList)
            if (token == TokenType.tok_identifier || token == TokenType.tok_keyword) {
                tokenList.push([token, identifierString])
                token = getNextToken()
                continue
            }

            if (token == TokenType.tok_float || token == TokenType.tok_number) {
                tokenList.push([token, numberString])
                token = getNextToken()
                continue
            }

            tokenList.push([token, current])
            token = getNextToken()
        }

        return tokenList
    }
}