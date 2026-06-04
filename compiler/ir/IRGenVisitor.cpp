#include "IRGenVisitor.h"

IRGenVisitor::IRGenVisitor(const std::map<std::string, int> &symbolTable)
{
    program.funcName = "main";
    program.symbols = symbolTable;
    // Les temporaires seront alloués après les variables nommées
    nextTempOffset = 0;
    for (auto &[_, off] : symbolTable)
        if (off < nextTempOffset)
            nextTempOffset = off;
}

std::string IRGenVisitor::newTemp()
{
    nextTempOffset -= 4;
    std::string name = "_t" + std::to_string(tempCount++);
    program.symbols[name] = nextTempOffset;
    return name;
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto *stmt : ctx->stmt())
        visit(stmt);
    visit(ctx->return_stmt());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item())
    {
        if (item->expr())
        {
            std::string src = str(visit(item->expr()));
            emit({IRInstr::COPY, item->ID()->getText(), src});
        }
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    emit({IRInstr::COPY, ctx->ID()->getText(), src});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    emit({IRInstr::RET, "", src});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitAddSubExpr(ifccParser::AddSubExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    IRInstr::Op op = (ctx->op->getText() == "+") ? IRInstr::ADD : IRInstr::SUB;
    emit({op, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitMulDivExpr(ifccParser::MulDivExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    std::string opStr = ctx->op->getText();
    IRInstr::Op op = (opStr == "*")   ? IRInstr::MUL
                     : (opStr == "/") ? IRInstr::DIV
                                      : IRInstr::MOD;
    emit({op, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitUnaryMinusExpr(ifccParser::UnaryMinusExprContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    std::string t = newTemp();
    emit({IRInstr::NEG, t, src});
    return t;
}

antlrcpp::Any IRGenVisitor::visitParenExpr(ifccParser::ParenExprContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any IRGenVisitor::visitIdExpr(ifccParser::IdExprContext *ctx)
{
    return ctx->ID()->getText();
}

antlrcpp::Any IRGenVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    std::string t = newTemp();
    int val = std::stoi(ctx->CONST()->getText());
    emit({IRInstr::LDCONST, t, "", "", val});
    return t;
}

antlrcpp::Any IRGenVisitor::visitCmpExpr(ifccParser::CmpExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    std::string opStr = ctx->op->getText();
    IRInstr::Op op = (opStr == "==")   ? IRInstr::CMP_EQ
                     : (opStr == "!=") ? IRInstr::CMP_NEQ
                     : (opStr == "<")  ? IRInstr::CMP_LT
                     : (opStr == ">")  ? IRInstr::CMP_GT
                                       : throw std::runtime_error("Unknown comparison operator");
    emit({op, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitNotExpr(ifccParser::NotExprContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    std::string t = newTemp();
    emit({IRInstr::NOT, t, src});
    return t;
}