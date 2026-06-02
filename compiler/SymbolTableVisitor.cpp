#include "SymbolTableVisitor.h"
#include <iostream>

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // Visit all statements and the return statement
    visitChildren(ctx);

    // Check declared but never used variables
    for (auto &[name, offset] : symbolTable)
    {
        if (!usedVars.count(name))
            std::cerr << "warning: variable '" << name << "' declared but never used\n";
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    std::string name = ctx->ID()->getText();

    if (symbolTable.count(name))
    {
        std::cerr << "error: variable '" << name << "' declared more than once\n";
        hasError = true;
    }
    else
    {
        nextOffset -= 4;
        symbolTable[name] = nextOffset;
        std::cerr << "debug: variable '" << name << "' -> index " << nextOffset << "\n";
    }

    // int a = expr;  →  check RHS if it's a variable
    if (ctx->expr())
    {
        this->visit(ctx->expr());
        usedVars.insert(name); // Mark the variable as used if it's initialized
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    // Check LHS variable is declared
    std::string lhs = ctx->ID()->getText();
    if (!symbolTable.count(lhs))
    {
        std::cerr << "error: variable '" << lhs << "' used but not declared\n";
        hasError = true;
    }
    else
    {
        usedVars.insert(lhs);
    }

    // Check RHS if it's a variable (not a constant)
    if (ctx->expr())
    {
        this->visit(ctx->expr());
    }

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIdExpr(ifccParser::IdExprContext *ctx)
{
    std::string name = ctx->ID()->getText();
    if (!symbolTable.count(name))
    {
        std::cerr << "error: variable '" << name << "' used but not declared\n";
        hasError = true;
    }
    else
    {
        usedVars.insert(name);
    }
    return 0;
}
