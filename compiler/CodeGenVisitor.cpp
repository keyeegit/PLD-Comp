#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    // count declarations to know how much stack space to reserve
    int varCount = 0;
    for (auto stmt : ctx->stmt())
        if (stmt->decl_stmt())
            varCount++;

    // each int = 4 bytes, round up to multiple of 16 for stack alignment
    int stackSize = ((varCount * 4 + 15) / 16) * 16;

#ifdef __APPLE__
    std::cout << ".globl _main\n";
    std::cout << "_main:\n";
    std::cout << "    stp x29, x30, [sp, #-16]!\n"; // save frame pointer and return address
    std::cout << "    mov x29, sp\n";               // set base pointer to current stack top
    if (stackSize > 0)
        std::cout << "    sub sp, sp, #" << stackSize << "\n"; // reserve space for local variables
#else
    std::cout << ".globl main\n";
    std::cout << "main:\n";
    std::cout << "    pushq %rbp\n";      // save caller's base pointer
    std::cout << "    movq %rsp, %rbp\n"; // set base pointer to current stack top
    if (stackSize > 0)
        std::cout << "    subq $" << stackSize << ", %rsp\n"; // reserve space for local variables
#endif

    // visit each statement (decl or assign) in order
    for (auto stmt : ctx->stmt())
        this->visit(stmt);

    // then visit the return statement
    this->visit(ctx->return_stmt());

#ifdef __APPLE__
    if (stackSize > 0)
        std::cout << "    add sp, sp, #" << stackSize << "\n"; // free local variable space
    std::cout << "    ldp x29, x30, [sp], #16\n";              // restore frame pointer and return address
#else
    if (stackSize > 0)
        std::cout << "    addq $" << stackSize << ", %rsp\n";
    std::cout << "    popq %rbp\n";
#endif
    std::cout << "    ret\n";

    return 0;
}

// int x;  or  int x = expr;
antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    std::string varName = ctx->ID()->getText();

    if (ctx->expr())
    {
        int lhsOffset = symbolTable[varName];
        this->visit(ctx->expr());
#ifdef __APPLE__
        std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
        std::cout << "    movl %eax, " << lhsOffset << "(%rbp)\n";
#endif
    }
    return 0;
}

// x = 8;  or  x = y;
antlrcpp::Any CodeGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string lhs = ctx->ID()->getText();
    int lhsOffset = symbolTable[lhs];

    this->visit(ctx->expr()); // result in w8 / eax
#ifdef __APPLE__
    std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
    std::cout << "    movl %eax, " << lhsOffset << "(%rbp)\n";
#endif
    return 0;
}

// return expr;
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    this->visit(ctx->expr()); // result in w8 / eax
#ifdef __APPLE__
    std::cout << "    mov w0, w8\n"; // move result to return register
#endif
    // on x86-64 the result is already in eax, nothing to do
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitUnaryMinusExpr(ifccParser::UnaryMinusExprContext *ctx)
{
    this->visit(ctx->expr()); // result in w8 / eax
#ifdef __APPLE__
    std::cout << "    neg w8, w8\n";
#else
    std::cout << "    negl %eax\n";
#endif
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitIdExpr(ifccParser::IdExprContext *ctx)
{
    std::string varName = ctx->ID()->getText();
#ifdef __APPLE__
    std::cout << "    ldr w8, [x29, #" << symbolTable[varName] << "]\n"; // load variable into w8
#else
    std::cout << "    movl " << symbolTable[varName] << "(%rbp), %eax\n"; // load variable into eax
#endif
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitConstExpr(ifccParser::ConstExprContext *ctx)
{
    int value = stoi(ctx->CONST()->getText());
#ifdef __APPLE__
    std::cout << "    mov w8, #" << value << "\n"; // load constant into w8
#else
    std::cout << "    movl $" << value << ", %eax\n"; // load constant into eax
#endif
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMulDivExpr(ifccParser::MulDivExprContext *ctx)
{
    this->visit(ctx->expr(0)); // résultat gauche → w8/eax
#ifdef __APPLE__
    std::cout << "    sub sp, sp, #16\n";
    std::cout << "    str w8, [sp]\n"; // sauvegarde gauche sur la pile
    this->visit(ctx->expr(1));         // résultat droit → w8
    std::cout << "    ldr w9, [sp]\n"; // récupère gauche dans w9
    std::cout << "    add sp, sp, #16\n";
    std::string op = ctx->op->getText();
    if (op == "*")
        std::cout << "    mul w8, w9, w8\n";
    else if (op == "/")
        std::cout << "    sdiv w8, w9, w8\n";
    else
        std::cout << "    sdiv w10, w9, w8\n" // %
                  << "    msub w8, w10, w8, w9\n";
#else
    std::cout << "    pushq %rax\n";
    this->visit(ctx->expr(1));
    std::cout << "    movl %eax, %ecx\n";
    std::cout << "    popq %rax\n";
    std::string op = ctx->op->getText();
    if (op == "*")
        std::cout << "    imull %ecx, %eax\n";
    else
    {
        std::cout << "    cltd\n"        // signe-extend eax → edx:eax
                  << "    idivl %ecx\n"; // eax=quotient, edx=reste
        if (op == "%")
            std::cout << "    movl %edx, %eax\n";
    }
#endif
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAddSubExpr(ifccParser::AddSubExprContext *ctx)
{
    this->visit(ctx->expr(0)); // résultat gauche → w8/eax
#ifdef __APPLE__
    std::cout << "    sub sp, sp, #16\n";
    std::cout << "    str w8, [sp]\n"; // sauvegarde gauche sur la pile
    this->visit(ctx->expr(1));         // résultat droit → w8
    std::cout << "    ldr w9, [sp]\n"; // récupère gauche dans w9
    std::cout << "    add sp, sp, #16\n";
    std::string op = ctx->op->getText();
    if (op == "+")
        std::cout << "    add w8, w9, w8\n";
    else
        std::cout << "    sub w8, w9, w8\n";
#else
    std::cout << "    pushq %rax\n";
    this->visit(ctx->expr(1));
    std::cout << "    movl %eax, %ecx\n";
    std::cout << "    popq %rax\n";
    std::string op = ctx->op->getText();
    if (op == "+")
        std::cout << "    addl %ecx, %eax\n";
    else
        std::cout << "    subl %ecx, %eax\n";
#endif
    return 0;
}