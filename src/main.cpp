#include <iostream>
#include <string>
#include "compiler/lexer.hpp"

using namespace ToastLang;

std::string src = 
    "func -> {\n"
    "   < 1\n"
    "}\n";

int main() {
    Lexer lexer = Lexer(src);
    std::string lexed = lexer.lex();
    return 0;
}