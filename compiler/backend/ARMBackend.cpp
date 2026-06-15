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

/*
  w8, w9, w10 are used as temporary registers
  w0 - w7 are used for function arguments and return value
  w0 is used for arguments and return value
*/
static void emitInstr(std::ostream &o, const IRInstr &i, const IRProgram &prog)
{
    auto adr = [&](const std::string &n)
    { return addr(n, prog); };

    switch (i.op)
    {
    case IRInstr::LDCONST:
        o << "    mov w8, #" << i.imm << "\n";      // Load immediate value into w8
        o << "    str w8, " << adr(i.dest) << "\n"; // Store w8 into the destination variable
        break;
    case IRInstr::COPY:
        o << "    ldr w8, " << adr(i.src1) << "\n"; // Load the value from src1 into w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::ADD:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    add w8, w8, w9\n"; // Add w8 and w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::SUB:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sub w8, w8, w9\n"; // Subtract w9 from w8, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MUL:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    mul w8, w8, w9\n"; // Multiply w8 and w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::DIV:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sdiv w8, w8, w9\n"; // Signed division of w8 by w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::MOD:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    sdiv w10, w8, w9\n";     // Signed division of w8 by w9, store quotient in w10
        o << "    msub w8, w10, w9, w8\n"; // Multiply quotient by divisor and subtract from dividend
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NEG:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    neg w8, w8\n"; // Negate the value in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::RET:
        if (!i.src1.empty())
            o << "    ldr w0, " << adr(i.src1) << "\n";
        o << "    b .Lexit_" << prog.funcName << "\n";
        break;
    case IRInstr::CMP_EQ:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";  // compare w8 and w9, update NZCV flags
        o << "    cset w8, eq\n"; // Set w8 to 1 if equal, 0 otherwise (Z = 1 if equal)
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_NEQ:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";  // compare w8 and w9, update NZCV flags
        o << "    cset w8, ne\n"; // Set w8 to 1 if not equal, 0 otherwise (Z = 0 if equal)
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_LT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";  // compare w8 and w9, update NZCV flags
        o << "    cset w8, lt\n"; // Set w8 to 1 if less than, 0 otherwise (N != V)
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_GT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    cmp w8, w9\n";  // compare w8 and w9, update NZCV flags
        o << "    cset w8, gt\n"; // Set w8 to 1 if greater than, 0 otherwise (Z = 0 and N = V)
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_AND:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    and w8, w8, w9\n"; // Bitwise AND of w8 and w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_OR:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    orr w8, w8, w9\n"; // Bitwise OR of w8 and w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::CMP_XOR:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    ldr w9, " << adr(i.src2) << "\n";
        o << "    eor w8, w8, w9\n"; // Bitwise XOR of w8 and w9, store result in w8
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::NOT:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    cmp w8, #0\n";  // Compare w8 with 0, update NZCV flags
        o << "    cset w8, eq\n"; // Set w8 to 1 if equal, 0 otherwise
        o << "    str w8, " << adr(i.dest) << "\n";
        break;
    case IRInstr::PUTCHAR:
        o << "    ldr w0, " << adr(i.src1) << "\n";
        o << "    bl _putchar\n"; // Call the _putchar function, which expects the character to print in w0
        break;
    case IRInstr::GETCHAR:
        o << "    bl _getchar\n"; // Call the _getchar function, which returns the read character in w0
        o << "    str w0, " << adr(i.dest) << "\n";
        break;
    case IRInstr::LABEL:
        o << i.dest << ":\n";
        break;

    // b -> branch (unconditional jump), no return address is stored
    // bl -> branch with link (function call), return address is stored in x30 (LR)
    case IRInstr::JMP:
        o << "    b " << i.dest << "\n"; // jump to the label specified in dest
        break;

    case IRInstr::CMP_CBR:
        o << "    ldr w8, " << adr(i.src1) << "\n";
        o << "    cbz w8, " << i.dest << "\n"; // if w8 is zero, jump to the label specified in dest
        break;
    case IRInstr::PARAM:
        o << "    str w" << i.imm << ", " << adr(i.dest) << "\n";
        break;
    case IRInstr::CALL:
        for (size_t idx = 0; idx < i.args.size(); ++idx)
            o << "    ldr w" << idx << ", " << adr(i.args[idx]) << "\n";
        o << "    bl _" << i.src1 << "\n";
        if (!i.dest.empty())
            o << "    str w0, " << adr(i.dest) << "\n";
        break;
    // w10 -> temporary register for array index
    // w11 -> temporary register for array value
    // x9 -> temporary register for base address of the array
    case IRInstr::STORE_ARRAY:
    {
        int baseOff = prog.symbols.at(i.dest);
        o << "    ldr w10, " << adr(i.src1) << "\n";
        o << "    ldr w11, " << adr(i.src2) << "\n";
        if (baseOff >= 0)
            o << "    add x9, x29, #" << baseOff << "\n";
        else
            o << "    sub x9, x29, #" << (-baseOff) << "\n";
        o << "    str w11, [x9, w10, uxtw #2]\n";
        break;
    }
    // w10 -> temporary register for array index
    // w11 -> temporary register for array value
    // x9 -> temporary register for base address of the array
    case IRInstr::LOAD_ARRAY:
    {
        int baseOff = prog.symbols.at(i.src1);
        o << "    ldr w10, " << adr(i.src2) << "\n";
        if (baseOff >= 0)
            o << "    add x9, x29, #" << baseOff << "\n";
        else
            o << "    sub x9, x29, #" << (-baseOff) << "\n";
        o << "    ldr w11, [x9, w10, uxtw #2]\n";
        o << "    str w11, " << adr(i.dest) << "\n";
        break;
    }
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
    o << ".Lexit_" << prog.funcName << ":\n";
    emitEpilogue(o, sz);
}
