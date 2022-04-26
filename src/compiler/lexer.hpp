#include <string>

#include <iostream>

enum TokenType {
    eof,
    words,
    string,
    number,
    identifier,
};

struct Token {
    TokenType name;
    std::string value;
};

namespace ToastLang {
    class Lexer {
        public:
            Lexer(std::string src);
            std::string lex();

        private:
            char getChar();
            std::string src;
            std::string identifierString;
            
            int iteration = 0;
            char currentChar;
    };
}

ToastLang::Lexer::Lexer(std::string src) {
    ToastLang::Lexer::src = src;
}

char ToastLang::Lexer::getChar() {
    return ToastLang::Lexer::src[iteration++];
}

std::string ToastLang::Lexer::lex() {
    ToastLang::Lexer::currentChar = ToastLang::Lexer::getChar();
    while (ToastLang::Lexer::currentChar != '\0') {
        if (isalpha(ToastLang::Lexer::currentChar)) {
            ToastLang::Lexer::identifierString = std::string("");
            while (isalnum((ToastLang::Lexer::currentChar))) {
                ToastLang::Lexer::identifierString += ToastLang::Lexer::currentChar;

                ToastLang::Lexer::currentChar = ToastLang::Lexer::getChar();
            }



            continue;
        }

        ToastLang::Lexer::currentChar = ToastLang::Lexer::getChar();
    }
    return std::string("test");
}