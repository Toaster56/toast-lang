#ifndef T_AST
#define T_AST

#include "./parser.h"

#include <iostream>
#include <vector>
#include <memory>
#include <map>

enum VarTypes {
    type_int,
    type_float,
    type_string,
    type_array,
    type_json,
    type_regex,
    type_fn,
    type_hex,
    type_auto,
    type_any,
    type_null,
    type_none
};

namespace ASTClasses {
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
    };
    
    class StringExpr : public ExprAST {
        std::string content;

        public:
            StringExpr(std::string content): content(content) {};
    };

    class IntExpr : public ExprAST {
        std::string value;
        
        public:
            IntExpr(std::string value): value(value) {};
    };

    class FloatExpr : public ExprAST {
        std::string value;
        
        public:
            FloatExpr(std::string value): value(value) {};
    };

    class VariableExpr : public ExprAST {
        std::string name;
        int type;
        std::unique_ptr<ExprAST> content;

        public:
            VariableExpr(std::string name, int type, std::unique_ptr<ExprAST> content): name(name), type(type), content(std::move(content)) {};
    };

    class FunctionExpr : public ExprAST {
        std::string name;
        int returnType;
        std::map<std::string, std::string> args;
        std::vector<ExprAST> body;

        public:
            FunctionExpr(std::string name,
                         int returnType,
                         std::map<std::string, std::string> args,
                         std::vector<ExprAST> body): name(name),
                                                     returnType(returnType),
                                                     args(args),
                                                     body(std::move(body)) {};
    };
}

class ASTGen {
    int currTok;
    Parser* parser;

    ASTClasses::FunctionExpr parseFunction();
    ASTClasses::ExprAST parseType();

    public:
        ASTGen(Parser* parser): parser(parser) {};
        ASTClasses::FunctionExpr gen();
};

#endif