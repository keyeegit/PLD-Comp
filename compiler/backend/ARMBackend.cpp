#include "ARMBackend.h"
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
    return "[x29, #" + std::to_string(prog.symbols.at(name)) + "]";
}

static void emitPrologue(std::ostream &o, const IRProgram &prog, int sz)
{
    o << ".globl _" << prog.funcName << "\n";
    o << "_" << prog.funcName << ":\n";
    o << "    stp x29, x30, [sp, #-16]!\n";
    o << "    mov x29, sp\n";
    if (sz > 0)
        o << "    sub sp, sp, #" << sz << "\n";
}

static void emitEpilogue(std::ostream &o, int sz)
{
    if (sz > 0)
        o << "    add sp, sp, #" << sz << "\n";
    o << "    ldp x29, x30, [sp], #16\n";
    o << "    ret\n";
}

static void emitInstr(std::ostream &o, const IRInstr &i, const IRProgram &prog)
{
    auto adr = [&](const std::string &n)
    { return addr(n, prog); };

    switch (i.op)
    {
    case IRInstr::LDCONST:
        o << "    mov w8, #" << i.imm << "\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::COPY:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::ADD:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    add w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::SUB:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sub w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MUL:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    mul w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::DIV:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sdiv w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MOD:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sdiv w10, w8, w9\n";
        o << "    msub w8, w10, w9, w8\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NEG:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    neg w8, w8\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::RET:
        o << "    ldr w0, " << adr(i.src1) << "\n";
        break;
    case IRInstr::CMP_EQ:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";
        o << "    cset w8, eq\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_NEQ:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";
        o << "    cset w8, ne\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_LT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";
        o << "    cset w8, lt\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_GT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";
        o << "    cset w8, gt\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_AND:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    and w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_OR:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    orr w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_XOR:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    eor w8, w8, w9\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NOT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    cmp w8, #0\n";
        o << "    cset w8, eq\n";
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::PUTCHAR:
        o << "    ldr w0, " << adr(i.src1) << "\n";
        o << "    bl _putchar\n";
        break;
    case IRInstr::GETCHAR:
        o << "    bl _getchar\n";
        o << "    str w0, " << adr(i.dest) << "\n";
        break;
    }
}

void ARMBackend::generate(std::ostream &o, const IRProgram &prog)
{
    int sz = stackSize(prog);
    emitPrologue(o, prog, sz);
    for (const auto &i : prog.instrs)
        emitInstr(o, i, prog);
    // If the last instruction is not a RET, we need to return 0 by default
    if (prog.instrs.empty() || prog.instrs.back().op != IRInstr::RET)
        o << "    mov w0, #0\n";
    emitEpilogue(o, sz);
}
