#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <map>
#include <set>
#include <string>

struct FuncInfo
{
    std::string returnType;
    std::vector<std::string> paramTypes;
};

class SymbolTableVisitor : public ifccBaseVisitor
{
public:
    virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;
    virtual antlrcpp::Any visitDecl_stmt(ifccParser::Decl_stmtContext *ctx) override;
    virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
    virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual antlrcpp::Any visitIdExpr(ifccParser::IdExprContext *ctx) override;
    virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
    virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
    virtual antlrcpp::Any visitCall_stmt(ifccParser::Call_stmtContext *ctx) override;
    virtual antlrcpp::Any visitCallExpr(ifccParser::CallExprContext *ctx) override;
    virtual antlrcpp::Any visitFunc_def(ifccParser::Func_defContext *ctx) override;

    std::map<std::string, FuncInfo> funcTable;
    std::map<std::string, std::map<std::string, int>> funcSymbolTables;

    std::map<std::string, int> globalSymbolTable;
    std::vector<std::map<std::string, std::string>> scopeStack;
    bool hasError = false;
    std::string currentFuncReturnType;

    std::string getUniqueName(const std::string &name)
    {
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
        {
            if (it->count(name))
                return (*it)[name];
        }
        return ""; // Non trouvé
    }

private:
    int nextOffset = 0;
    std::set<std::string> usedVars;
};
