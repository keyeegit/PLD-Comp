#pragma once
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "ir/IRInstr.h"
#include "frontend/SymbolTableVisitor.h"

class IRGenVisitor : public ifccBaseVisitor
{
public:
    explicit IRGenVisitor(
        const std::map<std::string, std::map<std::string, int>> &funcSymbols,
        const std::map<std::string, FuncInfo> &funcTables);

    const std::vector<IRProgram> &getPrograms() const { return programs; }

    antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
    antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    antlrcpp::Any visitAddSubExpr(ifccParser::AddSubExprContext *ctx) override;
    antlrcpp::Any visitMulDivExpr(ifccParser::MulDivExprContext *ctx) override;
    antlrcpp::Any visitUnaryMinusExpr(ifccParser::UnaryMinusExprContext *ctx) override;
    antlrcpp::Any visitParenExpr(ifccParser::ParenExprContext *ctx) override;
    antlrcpp::Any visitIdExpr(ifccParser::IdExprContext *ctx) override;
    antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;
    antlrcpp::Any visitRelCmpExpr(ifccParser::RelCmpExprContext *ctx) override;
    antlrcpp::Any visitEqCmpExpr(ifccParser::EqCmpExprContext *ctx) override;
    antlrcpp::Any visitBitAndExpr(ifccParser::BitAndExprContext *ctx) override;
    antlrcpp::Any visitBitXorExpr(ifccParser::BitXorExprContext *ctx) override;
    antlrcpp::Any visitBitOrExpr(ifccParser::BitOrExprContext *ctx) override;
    antlrcpp::Any visitNotExpr(ifccParser::NotExprContext *ctx) override;
    antlrcpp::Any visitCharConstExpr(ifccParser::CharConstExprContext *ctx) override;
    antlrcpp::Any visitGetcharExpr(ifccParser::GetcharExprContext *ctx) override;
    antlrcpp::Any visitPutchar_stmt(ifccParser::Putchar_stmtContext *ctx) override;
    antlrcpp::Any visitGetchar_stmt(ifccParser::Getchar_stmtContext *ctx) override;
    antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
    antlrcpp::Any visitFunc_def(ifccParser::Func_defContext *ctx) override;
    antlrcpp::Any visitCallExpr(ifccParser::CallExprContext *ctx) override;
    antlrcpp::Any visitCall_stmt(ifccParser::Call_stmtContext *ctx) override;

private:
    const std::map<std::string, std::map<std::string, int>> &funcSymbols;
    const std::map<std::string, FuncInfo> &funcTable;

    std::vector<IRProgram> programs;
    IRProgram *current = nullptr;
    int nextTempOffset = 0;
    int tempCount = 0;

    std::vector<std::map<std::string, std::string>> scopeStack;

    std::string getUniqueName(const std::string &name)
    {
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
        {
            if (it->count(name))
                return (*it)[name];
        }
        return name; // Au cas où, on replie sur le nom de base
    }

    std::string newTemp();
    void emit(IRInstr i) { current->instrs.push_back(i); }
    std::string str(antlrcpp::Any a) { return std::any_cast<std::string>(a); }

    int labelCount = 0;
    std::string newLabel();
};
