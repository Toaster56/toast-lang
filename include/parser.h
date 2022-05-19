#ifndef T_PARSER
#define T_PARSER

#include <iostream>
#include <string.h>
#include <string>

enum TokTypes {
    tok_eof,
    tok_identifier,
    tok_type,
    tok_number,
    tok_string,
    tok_equal,
    tok_comma,
    tok_dot,
    tok_plus,
    tok_minus,
    tok_divide,
    tok_multiply,
    tok_larrow,
    tok_rarrow,
    tok_lbracket,
    tok_rbracket,
    tok_lparan,
    tok_rparan
};

class Parser {
    std::string src;
    int currNum = 0;
    int last;

    std::string StringContainer;
    std::string IdentifierString;
    std::string NumberString;

    char getNext();

    public:
        Parser(std::string src): src(src) {Parser::getNext();};

        int getLast();
        int getToken();

        std::string getStringContainer() {
            return Parser::StringContainer;
        }

        std::string getIdString() {
            return Parser::IdentifierString;
        }

        std::string getNumString() {
            return Parser::NumberString;
        }
};
#endif