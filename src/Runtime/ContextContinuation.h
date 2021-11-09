#pragma once
#include "InstructionContext.h"

class ContextContinuation {
    public:
        InstructionContext* context;
        unsigned int instructionPointer;
};
