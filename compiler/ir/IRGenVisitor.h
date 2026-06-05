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
    antlrcpp::Any visitCmpExpr(ifccParser::CmpExprContext *ctx) override;
    antlrcpp::Any visitCmpBit(ifccParser::CmpBitContext *ctx) override;
    antlrcpp::Any visitNotExpr(ifccParser::NotExprContext *ctx) override;
    antlrcpp::Any visitCharConstExpr(ifccParser::CharConstExprContext *ctx) override;

private:
    IRProgram program;
    int nextTempOffset = 0;
    int tempCount = 0;

    std::string newTemp();
    void emit(IRInstr i) { program.instrs.push_back(i); }
    std::string str(antlrcpp::Any a) { return std::any_cast<std::string>(a); }
};
