#pragma once
#include "ir/IRBackend.h"

class x86Backend : public IRBackend {
public:
    void generate(std::ostream& o, const IRProgram& prog) override;
};
