#pragma once
#include <exception>
#include <string>

class UnboundFunctionError : public std::exception {
public:
    UnboundFunctionError(const std::string& functionName) : functionName(functionName){}

    std::string functionName;
};

