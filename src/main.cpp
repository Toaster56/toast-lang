#include <string>

#include <fstream>

#include "compiler/codegen.hpp"

int main() {
    std::fstream srcFile;
    srcFile.open("./src.t", std::ios::in);
    std::string src;
    if (srcFile.is_open()) {
        std::string srclbl;
        while (std::getline(srcFile, srclbl)) src += srclbl;
        srcFile.close();
    }

    std::cout << src << std::endl;
    ToastLang::CodeGen gen = ToastLang::CodeGen(src);

    return 0;
}