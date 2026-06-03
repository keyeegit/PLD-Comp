#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"

#include "frontend/SymbolTableVisitor.h"
#include "ir/IRInstr.h"
#include "ir/IRBackend.h"
#include "ir/IRGenVisitor.h"
#include "backend/x86Backend.h"
#include "backend/ARMBackend.h"

using namespace antlr4;

// Usage : ifcc [--arch x86|arm64] fichier.c
static std::string parseArgs(int argn, const char** argv, std::string& arch)
{
#ifdef __APPLE__
    arch = "arm64";
#else
    arch = "x86";
#endif

    if (argn == 4 && std::string(argv[1]) == "--arch") {
        arch = argv[2];
        return argv[3];
    }
    if (argn == 2)
        return argv[1];

    std::cerr << "usage: ifcc [--arch x86|arm64] path/to/file.c" << std::endl;
    exit(1);
}

int main(int argn, const char **argv)
{
    std::string arch;
    std::string filename = parseArgs(argn, argv, arch);

    std::stringstream in;
    {
        std::ifstream lecture(filename);
        if (!lecture.good()) {
            std::cerr << "error: cannot read file: " << filename << std::endl;
            exit(1);
        }
        in << lecture.rdbuf();
    }

    ANTLRInputStream input(in.str());
    ifccLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    tokens.fill();

    if (lexer.getNumberOfSyntaxErrors() != 0) {
        std::cerr << "error: syntax error during lexing" << std::endl;
        exit(1);
    }

    ifccParser parser(&tokens);
    tree::ParseTree* tree = parser.axiom();

    if (parser.getNumberOfSyntaxErrors() != 0) {
        std::cerr << "error: syntax error during parsing" << std::endl;
        exit(1);
    }

    // Passe 1 : analyse sémantique
    SymbolTableVisitor stv;
    stv.visit(tree);
    if (stv.hasError) {
        std::cerr << "error: semantic error" << std::endl;
        exit(1);
    }

    // Passe 2 : génération IR
    IRGenVisitor irgen(stv.symbolTable);
    irgen.visit(tree);

    // Passe 3 : génération assembleur
    x86Backend x86;
    ARMBackend arm;
    IRBackend* backend = (arch == "arm64") ? (IRBackend*)&arm : (IRBackend*)&x86;
    if (arch != "x86" && arch != "arm64") {
        std::cerr << "error: unknown arch '" << arch << "' (x86 or arm64)" << std::endl;
        exit(1);
    }
    backend->generate(std::cout, irgen.getProgram());

    return 0;
}
