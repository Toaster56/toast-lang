#include <string>

enum TokenTypes {
    tok_eof,

    tok_func,

    tok_identifier,

    tok_string,
    tok_number,
    tok_float
};

namespace ToastLang {
    class Lexer {
        std::string src;

        char getNextChar();

        public:
            char CurrentChar;
            std::string IdentifierString;
            double NumberValue;
            int Iteration = 0;

            Lexer(std::string src): src(src) {};
            int getNext();
    };
}

char ToastLang::Lexer::getNextChar() {
    return CurrentChar = src[Iteration++];
}

int ToastLang::Lexer::getNext() {
    getNextChar();
    while (isspace(CurrentChar))
        getNextChar();

    if (isalpha(CurrentChar)) {
        IdentifierString = CurrentChar;

        while (isalnum(getNextChar()))
            IdentifierString += CurrentChar;
        
        if (IdentifierString == "fn")
            return tok_func;
        
        return tok_identifier;
    }

    if (isdigit(CurrentChar) || CurrentChar == '.') {
        std::string NumberString;
        NumberValue = 0;

        bool f = false;

        while (isdigit(getNextChar()) || CurrentChar == '.') {
            if (CurrentChar == '.') f = true;
            NumberString += CurrentChar;
        }

        NumberValue = strtod(NumberString.c_str(), nullptr);

        return tok_number;
    }

    if (CurrentChar == '#') {
        while (CurrentChar != EOF && CurrentChar != '\n' && CurrentChar != '\r')
            getNextChar();
        
        if (CurrentChar != EOF)
            return getNext();
    }

    if (CurrentChar == EOF)
        return tok_eof;
    
    return CurrentChar;
}