#pragma once
#include <string>
#include "Bytecode.h"

class InstructionContext {
    public:
        Bytecode* instructions;
        unsigned int instructionCount;
        
        std::string name;

        ~InstructionContext();
};
