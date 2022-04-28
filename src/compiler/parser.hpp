#include <vector>
#include <memory>
#include <string>
#include <map>

#include <iostream>

#include "AST.hpp"
#include "lexer.hpp"

namespace ToastLang {
    class Parser {
        ToastLang::Lexer lexer;
        int getNextToken();
        
        std::unique_ptr<AST::ExprAST> ParseNumberExpr();
        std::unique_ptr<AST::ExprAST> ParseParenExpr();
        std::unique_ptr<AST::ExprAST> ParseIdentifierExpr();
        std::unique_ptr<AST::ExprAST> ParsePrimary();
        std::unique_ptr<AST::ExprAST> ParseExpression();
        std::unique_ptr<AST::ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<AST::ExprAST> LHS);

        std::unique_ptr<AST::PrototypeAST> ParsePrototype();
        std::unique_ptr<AST::FunctionAST> ParseFunction();

        std::unique_ptr<AST::FunctionAST> ParseTopLevelExpr();

        void HandleDefinition();
        void HandleTopLevelExpression();

        void mainLoop();

        int GetTokPrecedence();

        public:
            Parser(std::string src);
    };
}

int CurrentToken;
int ToastLang::Parser::getNextToken() {
    return CurrentToken = lexer.getNext();
};

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParseNumberExpr() {
    auto Result = std::make_unique<AST::NumberExprAST>(lexer.NumberValue);
    getNextToken();
    return std::move(Result);
}

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParseParenExpr() {
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;
    
    if (CurrentToken != ')')
        return nullptr;

    getNextToken();
    return V;
}

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParseIdentifierExpr() {
    std::string IdName = lexer.IdentifierString;

    getNextToken();

    if (CurrentToken != '(')
        return std::make_unique<AST::VariableExprAST>(IdName);
    
    getNextToken();
    std::vector<std::unique_ptr<AST::ExprAST>> Args;
    if (CurrentToken != ')') {
        while (1) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            
            if (CurrentToken == ')')
                break;
            
            getNextToken();
        }
    }

    getNextToken();

    return std::make_unique<AST::CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParsePrimary() {
    switch (CurrentToken) {
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
        default:
            return nullptr;
    }
}

std::map<char, int> BinopPrecedence;

int ToastLang::Parser::GetTokPrecedence() {
    if (!isascii(lexer.CurrentChar))
        return -1;
            
    int TokenPrec = BinopPrecedence[lexer.CurrentChar];
    if (TokenPrec <= 0) return -1;
    return TokenPrec;
}

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParseExpression() {
    auto LHS = ParsePrimary();
    
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<AST::ExprAST> ToastLang::Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<AST::ExprAST> LHS) {
    while (1) {
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec)
            return LHS;
        
        int BinOp = CurrentToken;
        getNextToken();

        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;
        
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec+1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        LHS = std::make_unique<AST::BinaryExprAST> (BinOp, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<AST::PrototypeAST> ToastLang::Parser::ParsePrototype() {
    if (CurrentToken != tok_identifier)
        return nullptr;
    
    std::string FnName = lexer.IdentifierString;
    
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(lexer.IdentifierString);
    if (CurrentToken != '{')
        return nullptr;
    
    getNextToken();

    return std::make_unique<AST::PrototypeAST>(FnName, ArgNames);
}

std::unique_ptr<AST::FunctionAST> ToastLang::Parser::ParseFunction() {
    getNextToken();
    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression()) {
        getNextToken();
        return std::make_unique<AST::FunctionAST>(std::move(Proto), std::move(E));
    }

    return nullptr;
}

std::unique_ptr<AST::FunctionAST> ToastLang::Parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto = std::make_unique<AST::PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<AST::FunctionAST>(std::move(Proto), std::move(E));
    }

    return nullptr;
}

void ToastLang::Parser::HandleDefinition() {
    if (auto FnAST = ParseFunction()) {
        if (auto *FnIR = FnAST->codegen()) {
            std::cout << "idfk created function ig:";
            FnIR->print(llvm::errs());
            std::cout << std::endl;
        }
    } else {
        getNextToken();
    }
}

void ToastLang::Parser::HandleTopLevelExpression() {
    if (auto FnAST = ParseTopLevelExpr()) {
    std::cout << "test" << std::endl;
        if (auto *FnIR = FnAST->codegen()) {
    std::cout << "test" << std::endl;
            std::cout << "Read top-level expression:";
            FnIR->print(llvm::errs());
            std::cout << std::endl;

            FnIR->eraseFromParent();
        }
    } else {
        getNextToken();
    }
}

void ToastLang::Parser::mainLoop() {
    getNextToken();
    while (1) {
        switch (CurrentToken) {
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_func:
                HandleDefinition();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

ToastLang::Parser::Parser(std::string src): lexer(ToastLang::Lexer(src)) {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;

    AST::init();
    mainLoop();

    AST::getTheModule()->print(llvm::errs(), nullptr);
}