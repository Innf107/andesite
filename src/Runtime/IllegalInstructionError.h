#pragma once
#include "Bytecode.h"
#include <exception>
#include <string>

class IllegalInstructionError : public std::exception {
public:
    IllegalInstructionError(Bytecode bytecode, unsigned int instrPtr, const std::string& contextName)
        : bytecode(bytecode), instrPtr(instrPtr), contextName(contextName){
            std::ostringstream messageStream;
            messageStream << "Illegal instruction: " << bytecode.toString() << std::endl
                          << "in context '" << contextName << "'" << std::endl 
                          << "at instruction pointer " << instrPtr << std::endl;
            message = messageStream.str();
        };

    Bytecode bytecode;
    unsigned int instrPtr;
    std::string contextName;

    std::string message;

    const char* what() const noexcept override {
        return message.c_str();
    }
};

