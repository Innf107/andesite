#pragma once
#include <exception>
#include <string>

class AlreadyDefinedFunctionError : public std::exception {
public:
    AlreadyDefinedFunctionError(const std::string& functionName) : functionName(functionName){}

    std::string functionName;

    

};

