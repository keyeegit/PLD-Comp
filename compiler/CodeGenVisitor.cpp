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
    std::cout << "    popq %rbp\n"; // restore caller's base pointer
#endif
    std::cout << "    ret\n";

    return 0;
}

// int x;  or  int x = expr;
antlrcpp::Any CodeGenVisitor::visitDecl_stmt(ifccParser::Decl_stmtContext *ctx)
{
    std::string varName = ctx->ID()->getText();
    nextOffset -= 4;
    symbolTable[varName] = nextOffset;

    // int x = 42;  or  int x = y;
    if (ctx->expr())
    {
        int lhsOffset = symbolTable[varName];
        if (ctx->expr()->CONST())
        {
            int value = stoi(ctx->expr()->CONST()->getText());
#ifdef __APPLE__
            std::cout << "    mov w8, #" << value << "\n";
            std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
            std::cout << "    movl $" << value << ", " << lhsOffset << "(%rbp)\n";
#endif
        }
        else
        {
            std::string rhs = ctx->expr()->ID()->getText();
            int rhsOffset = symbolTable[rhs];
#ifdef __APPLE__
            std::cout << "    ldr w8, [x29, #" << rhsOffset << "]\n";
            std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
            std::cout << "    movl " << rhsOffset << "(%rbp), %eax\n";
            std::cout << "    movl %eax, " << lhsOffset << "(%rbp)\n";
#endif
        }
    }
    return 0;
}

// x = 8;  or  x = y;
antlrcpp::Any CodeGenVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx)
{
    std::string lhs = ctx->ID()->getText();
    int lhsOffset = symbolTable[lhs];

    if (ctx->expr()->CONST())
    {
        // x = 42;
        int value = stoi(ctx->expr()->CONST()->getText());
#ifdef __APPLE__
        std::cout << "    mov w8, #" << value << "\n";
        std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
        std::cout << "    movl $" << value << ", " << lhsOffset << "(%rbp)\n";
#endif
    }
    else
    {
        // x = y;
        std::string rhs = ctx->expr()->ID()->getText();
        int rhsOffset = symbolTable[rhs];
#ifdef __APPLE__
        std::cout << "    ldr w8, [x29, #" << rhsOffset << "]\n";
        std::cout << "    str w8, [x29, #" << lhsOffset << "]\n";
#else
        std::cout << "    movl " << rhsOffset << "(%rbp), %eax\n";
        std::cout << "    movl %eax, " << lhsOffset << "(%rbp)\n";
#endif
    }
    return 0;
}

// return x; or return 42;
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    if (ctx->CONST())
    {
        // returning a literal number
        int retval = stoi(ctx->CONST()->getText());
#ifdef __APPLE__
        std::cout << "    mov w0, #" << retval << "\n"; // put value in return register
#else
        std::cout << "    movl $" << retval << ", %eax\n"; // put value in return register
#endif
    }
    else
    {
        // returning a variable — load it from the stack
        std::string varName = ctx->ID()->getText();
        int offset = symbolTable[varName]; // e.g. -4
#ifdef __APPLE__
        std::cout << "    ldr w0, [x29, #" << offset << "]\n"; // load x from memory into w0
#else
        std::cout << "    movl " << offset << "(%rbp), %eax\n"; // load x from memory into eax
#endif
    }
    return 0;
}
