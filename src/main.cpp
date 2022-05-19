#include "../include/ast.h"
#include "../include/parser.h"
#include "../include/util.h"

#include <iostream>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) exit(0);
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            i++;
            std::string s = readAsciiFile(argv[i]);
            Parser parser = Parser(s);
            ASTGen generator = ASTGen(&parser);
            generator.gen();
        }
    }
    return 0;
}