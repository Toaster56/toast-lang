#include "../include/ast.h"

ASTClasses::FunctionExpr ASTGen::parseFunction() {
    ASTGen::parser->getToken();
    std::string name = ASTGen::parser->getIdString();
    ASTGen::parser->getToken();
    if (ASTGen::parser->getLast() != tok_equal) {
        std::cerr << "ERROR: Expected \"=\" but got \"" << ASTGen::parser->getLast() << "\"" << std::endl;
        exit(1);
    }
    ASTGen::parser->getToken();

    std::map<std::string, std::string> args;

    std::vector<ASTClasses::ExprAST> body;

    int returnType;

    return ASTClasses::FunctionExpr(name, returnType, args, body);
}

ASTClasses::FunctionExpr ASTGen::gen() {
    std::vector<ASTClasses::ExprAST> ProgramBody;

    while (ASTGen::parser->getLast() != '\0') {
        int token = ASTGen::parser->getToken();
        
        if (token == tok_type) {
            std::string type = ASTGen::parser->getIdString();
            
            if (type == "fn") {
                ASTClasses::FunctionExpr func = parseFunction();

                ProgramBody.push_back(func);
                continue;
            }
            
            continue;
        }
    }

    return ASTClasses::FunctionExpr("Program", 0, {}, ProgramBody);
}