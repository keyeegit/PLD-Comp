#pragma once
#include "ir/IRBackend.h"

class ARMBackend : public IRBackend {
public:
    void generate(std::ostream& o, const IRProgram& prog) override;
};
