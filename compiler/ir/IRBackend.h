#pragma once
#include <iostream>
#include "IRInstr.h"

class IRBackend
{
public:
    virtual ~IRBackend() = default;
    virtual void generate(std::ostream &o, const IRProgram &prog) = 0;
};
