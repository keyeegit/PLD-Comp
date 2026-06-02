#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <map>    // for std::map
#include <string> // for std::string

class CodeGenVisitor : public ifccBaseVisitor
{
public:
        CodeGenVisitor(std::map<std::string, int> symbolTable) : symbolTable(symbolTable) {}

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
        virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
        virtual antlrcpp::Any visitMulDivExpr(ifccParser::MulDivExprContext *ctx) override;
        virtual antlrcpp::Any visitAddSubExpr(ifccParser::AddSubExprContext *ctx) override;
        virtual antlrcpp::Any visitUnaryMinusExpr(ifccParser::UnaryMinusExprContext *ctx) override;
        virtual antlrcpp::Any visitIdExpr(ifccParser::IdExprContext *ctx) override;
        virtual antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;

private:
        std::map<std::string, int> symbolTable;
};
