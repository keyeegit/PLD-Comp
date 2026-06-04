#pragma once
#include <string>
#include <vector>
#include <map>

struct IRInstr
{
    enum Op
    {
        LDCONST,
        COPY,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        NEG,
        RET,
        CMP_EQ,
        CMP_NEQ,
        CMP_LT,
        CMP_GT,
        NOT,
        CMP_AND,
        CMP_OR,
        CMP_XOR,
    };
    Op op;
    std::string dest;
    std::string src1;
    std::string src2;
    int imm = 0;
};

struct IRProgram
{
    std::string funcName;
    std::map<std::string, int> symbols;
    std::vector<IRInstr> instrs;
};
