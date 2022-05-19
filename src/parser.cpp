#include "../include/parser.h"

char Parser::getNext() {
    return Parser::last = Parser::src[Parser::currNum++];
}

int Parser::getLast() {
    return Parser::last;
}

int Parser::getToken() {
    while (isspace(Parser::last)) Parser::getNext();
    
    if (Parser::last == '\0') return tok_eof;

    if (isalpha(Parser::last)) {
        Parser::IdentifierString = Parser::last;
        while (isalnum(Parser::getNext()))
            Parser::IdentifierString += Parser::last;
        
        if (Parser::IdentifierString == "int"    || Parser::IdentifierString == "float"
         || Parser::IdentifierString == "string" || Parser::IdentifierString == "array"
         || Parser::IdentifierString == "json"   || Parser::IdentifierString == "regex"
         || Parser::IdentifierString == "fn"     || Parser::IdentifierString == "hex"
         || Parser::IdentifierString == "auto"   || Parser::IdentifierString == "any"
         || Parser::IdentifierString == "null"   || Parser::IdentifierString == "none"
         )
            return tok_type;
        return tok_identifier;
    }

    if (isdigit(Parser::last) || Parser::last == '.') {
        Parser::NumberString = "";
        do {
            Parser::NumberString += Parser::last;
            Parser::getNext();
        } while (isdigit(Parser::last) || Parser::last == '.');

        if (Parser::NumberString == ".") return tok_dot;
        
        return tok_number;
    }

    if (Parser::last == '=') {
        Parser::getNext();
        return tok_equal;
    }

    if (Parser::last == ',') {
        Parser::getNext();
        return tok_comma;
    }

    if (Parser::last == '+') {
        Parser::getNext();
        return tok_plus;
    }

    if (Parser::last == '-') {
        Parser::getNext();
        return tok_minus;
    }

    if (Parser::last == '/') {
        Parser::getNext();
        return tok_divide;
    }

    if (Parser::last == '*') {
        Parser::getNext();
        return tok_multiply;
    }

    if (Parser::last == '<') {
        Parser::getNext();
        return tok_larrow;
    }

    if (Parser::last == '>') {
        Parser::getNext();
        return tok_rarrow;
    }

    if (Parser::last == '{') {
        Parser::getNext();
        return tok_lbracket;
    }

    if (Parser::last == '}') {
        Parser::getNext();
        return tok_rbracket;
    }

    if (Parser::last == '(') {
        Parser::getNext();
        return tok_lparan;
    }

    if (Parser::last == ')') {
        Parser::getNext();
        return tok_rparan;
    }

    if (Parser::last == ')') {
        Parser::getNext();
        return tok_rparan;
    }

    if (Parser::last == '\'' || Parser::last == '"') {
        char quote = Parser::last;        

        Parser::StringContainer = "";
        while (Parser::getNext() != quote) Parser::StringContainer += Parser::last;
        Parser::getNext();

        return tok_string;
    }

    if (Parser::last == '#') {
        do Parser::getNext();
        while (Parser::last != '\0' && Parser::last != '\n' && Parser::last != '\r');

        if (Parser::last != '\0') return Parser::getNext();
    }
    
    std::cerr << "ERROR: Unknown character \"" << (char)Parser::last << "\"" << std::endl;
    exit(1);
}