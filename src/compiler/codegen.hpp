#include "parser.hpp"

namespace ToastLang {
    class CodeGen {
        public:
            CodeGen(std::string src);
    };
}

ToastLang::CodeGen::CodeGen(std::string src) {
    ToastLang::Parser parser = ToastLang::Parser(src);
}