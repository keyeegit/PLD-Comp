#include "x86Backend.h"
#include <iostream>

static int stackSize(const IRProgram &prog)
{
    int minOff = 0;
    for (auto &[_, off] : prog.symbols)
        if (off < minOff)
            minOff = off;
    return ((-minOff + 15) / 16) * 16;
}

static std::string addr(const std::string &name, const IRProgram &prog)
{
    return std::to_string(prog.symbols.at(name)) + "(%rbp)";
}

static void emitPrologue(std::ostream &o, const IRProgram &prog, int sz)
{
    o << ".globl " << prog.funcName << "\n";
    o << prog.funcName << ":\n";
    o << "    pushq %rbp\n";
    o << "    movq %rsp, %rbp\n";
    if (sz > 0)
        o << "    subq $" << sz << ", %rsp\n";
}

static void emitEpilogue(std::ostream &o, int sz)
{
    if (sz > 0)
        o << "    addq $" << sz << ", %rsp\n";
    o << "    popq %rbp\n";
    o << "    ret\n";
}

static void emitInstr(std::ostream &o, const IRInstr &i, const IRProgram &prog)
{
    auto adr = [&](const std::string &n)
    { return addr(n, prog); };

    switch (i.op)
    {
    case IRInstr::LDCONST:
        o << "    movl $" << i.imm << ", " << adr(i.dest) << "\n";
        break;
    case IRInstr::COPY:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::ADD:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    addl %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::SUB:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    subl %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MUL:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    imull %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::DIV:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cltd\n";
        o << "    idivl %ecx\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MOD:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cltd\n";
        o << "    idivl %ecx\n";
        o << "    movl %edx, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NEG:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    negl %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::RET:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        break;
    case IRInstr::CMP_EQ:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cmpl %ecx, %eax\n";
        o << "    sete %al\n";
        o << "    movzbl %al, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_NEQ:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cmpl %ecx, %eax\n";
        o << "    setne %al\n";
        o << "    movzbl %al, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_LT:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cmpl %ecx, %eax\n";
        o << "    setl %al\n";
        o << "    movzbl %al, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_GT:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    cmpl %ecx, %eax\n";
        o << "    setg %al\n";
        o << "    movzbl %al, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_AND:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    andl %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_OR:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    orl %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_XOR:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    movl " << adr(i.src2) << ", %ecx\n";
        o << "    xorl %ecx, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NOT:
        o << "    movl " << adr(i.src1) << ", %eax\n";
        o << "    cmpl $0, %eax\n";
        o << "    sete %al\n";
        o << "    movzbl %al, %eax\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::PUTCHAR:
        o << "    movl " << adr(i.src1) << ", %edi\n";
        o << "    call putchar\n";
        break;
    case IRInstr::GETCHAR:
        o << "    call getchar\n";
        o << "    movl %eax, " << adr(i.dest) << "\n";
        break;
    case IRInstr::LABEL:
        o << i.dest << ":\n";
        break;
    case IRInstr::JMP:
        o << "    jmp " << i.dest << "\n";
        break;
    case IRInstr::CMP_CBR:
        o << "    cmpl $0, " << adr(i.src1) << "\n";
        o << "    je " << i.dest << "\n";
        break;
    }
}

void x86Backend::generate(std::ostream &o, const IRProgram &prog)
{
    int sz = stackSize(prog);
    emitPrologue(o, prog, sz);
    for (const auto &i : prog.instrs)
        emitInstr(o, i, prog);
    // If the last instruction is not a RET, we need to return 0 by default
    if (prog.instrs.empty() || prog.instrs.back().op != IRInstr::RET)
        o << "    movl $0, %eax\n";
    emitEpilogue(o, sz);
}
