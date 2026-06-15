#include "IRGenVisitor.h"

IRGenVisitor::IRGenVisitor(const std::map<std::string, int> &symbolTable)
{
    program.funcName = "main";
    program.symbols = symbolTable;
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

std::string IRGenVisitor::newLabel()
{
    return ".L" + std::to_string(labelCount++);
}

antlrcpp::Any IRGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    visit(ctx->block());
    return 0;
}

antlrcpp::Any IRGenVisitor::visitBlock(ifccParser::BlockContext *ctx)
{
    scopeStack.push_back({}); // Nouveau sous-scope local

    for (auto *stmt : ctx->stmt())
        visit(stmt);

    scopeStack.pop_back(); // Ferme le sous-scope

    return 0;
}

antlrcpp::Any IRGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    for (auto *item : ctx->decl_item())
    {
        std::string name = item->ID()->getText();

        // On reconstruit le même nom unique exact (ex: a_0, a_1...)
        std::string uniqueName = name + "_b" + std::to_string(scopeStack.size());
        scopeStack.back()[name] = uniqueName;

        if (item->expr())
        {
            std::string src = str(visit(item->expr()));
            emit({IRInstr::COPY, uniqueName, src}); // On utilise le nom unique !
        }
    }
    return 0;
}

antlrcpp::Any IRGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    std::string uniqueName = getUniqueName(ctx->ID()->getText()); // Trouve le bon nom unique courant
    emit({IRInstr::COPY, uniqueName, src});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitIdExpr(ifccParser::IdExprContext *ctx)
{
    // Quand on lit une variable, on renvoie son nom unique associé dans le scope courant
    return getUniqueName(ctx->ID()->getText());
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

antlrcpp::Any IRGenVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    std::string t = newTemp();
    int val = std::stoi(ctx->CONST()->getText());
    emit({IRInstr::LDCONST, t, "", "", val});
    return t;
}

antlrcpp::Any IRGenVisitor::visitRelCmpExpr(ifccParser::RelCmpExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    std::string opStr = ctx->op->getText();
    IRInstr::Op op = (opStr == "<")   ? IRInstr::CMP_LT
                     : (opStr == ">") ? IRInstr::CMP_GT
                                      : throw std::runtime_error("Unknown comparison operator");
    emit({op, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitEqCmpExpr(ifccParser::EqCmpExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    std::string opStr = ctx->op->getText();
    IRInstr::Op op = (opStr == "==")   ? IRInstr::CMP_EQ
                     : (opStr == "!=") ? IRInstr::CMP_NEQ
                                       : throw std::runtime_error("Unknown comparison operator");
    emit({op, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitBitAndExpr(ifccParser::BitAndExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    emit({IRInstr::CMP_AND, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitBitXorExpr(ifccParser::BitXorExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    emit({IRInstr::CMP_XOR, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitBitOrExpr(ifccParser::BitOrExprContext *ctx)
{
    std::string l = str(visit(ctx->expr(0)));
    std::string r = str(visit(ctx->expr(1)));
    std::string t = newTemp();
    emit({IRInstr::CMP_OR, t, l, r});
    return t;
}

antlrcpp::Any IRGenVisitor::visitNotExpr(ifccParser::NotExprContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    std::string t = newTemp();
    emit({IRInstr::NOT, t, src});
    return t;
}

antlrcpp::Any IRGenVisitor::visitCharConstExpr(ifccParser::CharConstExprContext *ctx)
{
    std::string t = newTemp();
    std::string charText = ctx->CHAR_CONST()->getText();
    char c = charText[1];
    emit({IRInstr::LDCONST, t, "", "", static_cast<int>(c)});
    return t;
}

antlrcpp::Any IRGenVisitor::visitPutchar_stmt(ifccParser::Putchar_stmtContext *ctx)
{
    std::string src = str(visit(ctx->expr()));
    emit({IRInstr::PUTCHAR, "", src});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitGetcharExpr(ifccParser::GetcharExprContext *ctx)
{
    std::string t = newTemp();
    emit({IRInstr::GETCHAR, t});
    return t;
}

antlrcpp::Any IRGenVisitor::visitGetchar_stmt(ifccParser::Getchar_stmtContext *ctx)
{
    std::string t = newTemp();
    emit({IRInstr::GETCHAR, t});
    return 0;
}

antlrcpp::Any IRGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx)
{
    std::string cond = str(visit(ctx->expr())); // Évalue la condition (donne un temp ou une var)
    std::string labelElse = newLabel();
    std::string labelEnd = newLabel();

    // 1. Si la condition est FAUSSE (égale à 0), on saute au bloc Else (ou End s'il n'y a pas de else)
    emit({IRInstr::CMP_CBR, labelElse, cond, ""}); 

    // 2. On visite le corps du 'then'
    visit(ctx->stmt(0));

    // S'il y a un 'else'
    if (ctx->stmt().size() > 1) {
        // Si on a exécuté le 'then', on doit sauter par-dessus le 'else'
        emit({IRInstr::JMP, labelEnd, "", ""}); 
        
        // On pose le label de l'alternative Else
        emit({IRInstr::LABEL, labelElse, "", ""});
        
        // On visite le corps du 'else'
        visit(ctx->stmt(1));
        
        // On pose le label de fin général
        emit({IRInstr::LABEL, labelEnd, "", ""});
    } 
    else {
        // S'il n'y a pas de 'else', le labelElse est en réalité le label de fin
        emit({IRInstr::LABEL, labelElse, "", ""});
    }

    return 0;
}