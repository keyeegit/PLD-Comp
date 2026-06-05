#pragma once
#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "ir/IRInstr.h"

class IRGenVisitor : public ifccBaseVisitor
{
public:
    explicit IRGenVisitor(const std::map<std::string, int> &symbolTable);
    const IRProgram &getProgram() const { return program; }

    antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
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

private:
    IRProgram program;
    int nextTempOffset = 0;
    int tempCount = 0;

    std::string newTemp();
    void emit(IRInstr i) { program.instrs.push_back(i); }
    std::string str(antlrcpp::Any a) { return std::any_cast<std::string>(a); }
};
