#include "../include/util.h"

std::string readAsciiFile(std::string src) {
    std::ifstream f;
    f.open(src, std::ios::in);
    if (!f.is_open()) exit(1);
    
    std::string c;
    std::string line;

    while (!f.eof()) {
        getline(f, line);
        c.append(line + "\n");
    }

    c += '\0';

    f.close();
    return c;
}