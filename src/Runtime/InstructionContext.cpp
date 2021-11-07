#include "Runtime/InstructionContext.h"

InstructionContext::~InstructionContext(){
    delete[] instructions;
}

