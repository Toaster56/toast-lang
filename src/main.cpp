#include <string>

#include "compiler/parser.hpp"

std::string src = 
    "fn main a b {"
    "   a + b"
    "}";

int main() {
    ToastLang::Parser parser = ToastLang::Parser(src);

    return 0;
}