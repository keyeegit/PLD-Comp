#include "SymbolTableVisitor.h"
#include <iostream>

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    visit(ctx->block());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    scopeStack.push_back({});

    for (auto *stmt : ctx->stmt())
    {
        this->visit(stmt);
    }

    scopeStack.pop_back();

    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item())
    {
        std::string name = item->ID()->getText();

        // Interdit de redéclarer dans le MÊME bloc
        if (scopeStack.back().count(name))
        {
            std::cerr << "error: variable '" << name << "' declared more than once\n";
            hasError = true;
        }
        else
        {
            // On génère un nom unique pour l'IR (ex: "a_0")
            std::string uniqueName = name + "_b" + std::to_string(scopeStack.size());

            // On lie le nom C au nom unique dans le bloc courant
            scopeStack.back()[name] = uniqueName;

            // On alloue son offset dans la table globale (qui ne sera JAMAIS vidée)
            nextOffset -= 4;
            globalSymbolTable[uniqueName] = nextOffset;
        }

        if (item->expr())
        {
            this->visit(item->expr());
            usedVars.insert(getUniqueName(name));
        }
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string lhs = ctx->ID()->getText();
    std::string uniqueName = getUniqueName(lhs);

    if (uniqueName == "")
    {
        std::cerr << "error: variable '" << lhs << "' used but not declared\n";
        hasError = true;
    }
    else
    {
        usedVars.insert(uniqueName);
    }

    if (ctx->expr())
        this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIdExpr(ifccParser::IdExprContext *ctx)
{
    std::string name = ctx->ID()->getText();
    std::string uniqueName = getUniqueName(name);

    if (uniqueName == "")
    {
        std::cerr << "error: variable '" << name << "' used but not declared\n";
        hasError = true;
    }
    else
    {
        usedVars.insert(uniqueName);
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    // Le return visite simplement son expression, peu importe où il est placé
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    visit(ctx->expr());
    visit(ctx->stmt(0));
    if (ctx->stmt().size() > 1)
    {
        visit(ctx->stmt(1));
    }
    return 0;
}

antlrcpp::Any SymbolTableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx)
{
    visit(ctx->expr());
    visit(ctx->stmt());
    return 0;
}
