#include "compiler/parser.hpp"

std::string src = 
    "fn main a b {"
    "   < a + b"
    "}";

int main() {
    ToastLang::Parser parser = ToastLang::Parser(std::move(src));
    return 0;
}