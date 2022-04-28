#include <string>
#include <iostream>

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
        int Iteration = 0;

        char getNextChar();

        public:
            char CurrentChar;
            std::string IdentifierString;
            double NumberValue;

            Lexer(std::string src): src(src) {};
            int getNext();
    };
}