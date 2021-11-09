#include "Runtime/InstructionContext.h"
#include <iostream>

InstructionContext::~InstructionContext(){
    delete[] instructions;
}

InstructionContext::InstructionContext(const InstructionContext& other){
    instructionCount = other.instructionCount;
    instructions = new Bytecode[instructionCount];

    for (unsigned int i = 0; i < instructionCount; i++){
        instructions[i] = other.instructions[i];
    }
}