#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::Module> TheModule;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::map<std::string, llvm::Value *> NamedValues;

namespace AST {
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
            virtual llvm::Value *codegen() = 0;
    };

    class NumberExprAST: public ExprAST {
        double Val;

        public:
            NumberExprAST(double Val): Val(Val) {};
            llvm::Value *codegen() override {
                return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
            };
    };

    class VariableExprAST: public ExprAST {
        std::string Name;

        public:
            VariableExprAST(const std::string &Name): Name(Name) {};
            llvm::Value *codegen() override {
                llvm::Value *V = NamedValues[Name];
                if (!V)
                    return nullptr;
                return V;
            };
    };

    class BinaryExprAST: public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

        public:
            BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {};
            llvm::Value *codegen() override {
                llvm::Value *L = LHS->codegen();
                llvm::Value *R = LHS->codegen();

                if (!L || !R)
                    return nullptr;

                switch (Op) {
                    case '+':
                        return Builder->CreateFAdd(L, R, "addtmp");
                    case '-':
                        return Builder->CreateFSub(L, R, "subtmp");
                    case '*':
                        return Builder->CreateFMul(L, R, "multmp");
                    case '<':
                        L = Builder->CreateFCmpULT(L, R, "cmptmp");
                        
                        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
                    default:
                        return nullptr;
                }
            };
    };

    class CallExprAST: public ExprAST {
        std::string CallName;
        std::vector<std::unique_ptr<ExprAST>> Args;

        public:
            CallExprAST(const std::string &CallName, std::vector<std::unique_ptr<ExprAST>> Args): CallName(CallName), Args(std::move(Args)) {};
            llvm::Value *codegen() override {
                llvm::Function *CallNameF = TheModule->getFunction(CallName);

                if (!CallNameF)
                    return nullptr;
                
                if (CallNameF->arg_size() != Args.size())
                    return nullptr;
                
                std::vector<llvm::Value *> ArgsV;
                for (unsigned i = 0, e = Args.size(); i != e; ++i) {
                    ArgsV.push_back(Args[i]->codegen());
                    if (!ArgsV.back())
                        return nullptr;
                }
                
                return Builder->CreateCall(CallNameF, ArgsV, "calltmp");
            };
    };

    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;

        public:
            PrototypeAST(const std::string &Name, std::vector<std::string> Args): Name(Name), Args(std::move(Args)) {};
            llvm::Function *codegen() {
                std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));
                llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

                llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());
                
                unsigned Idx = 0;
                for (auto &Arg : F->args())
                    Arg.setName(Args[Idx++]);

                return F;
            };
            const std::string &getName() const { return Name; };
    };

    class FunctionAST {
        std::unique_ptr<PrototypeAST> Prototype;
        std::unique_ptr<ExprAST> Body;

        public:
            FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExprAST> Body): Prototype(std::move(Prototype)), Body(std::move(Body)) {};
            llvm::Function *codegen() {
                llvm::Function *TheFunction = TheModule->getFunction(Prototype->getName());

                if (!TheFunction)
                    TheFunction = Prototype->codegen();
                
                if (!TheFunction)
                    return nullptr;
                
                llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
                Builder->SetInsertPoint(BB);

                NamedValues.clear();
                for (auto &Arg : TheFunction->args())
                    NamedValues[std::string(Arg.getName())] = &Arg;

                if (llvm::Value *RetVal = Body->codegen()) {
                    Builder->CreateRet(RetVal);
                    llvm::verifyFunction(*TheFunction);
                    return TheFunction;
                }
                
                TheFunction->eraseFromParent();
                return nullptr;
            };
    };

    void init() {
        TheContext = std::make_unique<llvm::LLVMContext>();
        TheModule = std::make_unique<llvm::Module>("ToastLang jit ig", *TheContext);

        Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    };

    std::unique_ptr<llvm::Module> getTheModule() {
        return std::move(TheModule);
    }
}