#pragma once
#include <string>
#include <vector>
#include <map>

struct IRInstr
{
    enum Op
    {
        LDCONST,     // Load constant into a variable
        COPY,        // Copy value from one variable to another
        ADD,         // Add two values
        SUB,         // Subtract two values
        MUL,         // Multiply two values
        DIV,         // Divide two values
        MOD,         // Modulo operation
        NEG,         // Negate a value
        RET,         // Return from function
        CMP_EQ,      // Compare equal
        CMP_NEQ,     // Compare not equal
        CMP_LT,      // Compare less than
        CMP_GT,      // Compare greater than
        NOT,         // Logical not
        CMP_AND,     // Logical and
        CMP_OR,      // Logical or
        CMP_XOR,     // Logical xor
        PUTCHAR,     // Put character
        GETCHAR,     // Get character
        LABEL,       // Label
        CMP_CBR,     // Conditional branch
        JMP,         // Jump
        PARAM,       // Parameter
        CALL,        // Function call
        STORE_ARRAY, // Store array element
        LOAD_ARRAY,  // Load array element
    };
    Op op;                         // The operation type of the instruction
    std::string dest;              // The destination variable for the instruction
    std::string src1;              // The first source variable for the instruction
    std::string src2;              // The second source variable for the instruction
    int imm = 0;                   // Immediate value for the instruction
    std::vector<std::string> args; // Arguments for function calls
};

struct IRProgram
{
    std::string funcName;
    std::string returnType;
    std::map<std::string, int> symbols;
    std::vector<IRInstr> instrs;
};
