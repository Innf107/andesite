#pragma once
#include <string>

class RuntimeError {
    public:
        std::string message;
        RuntimeError(const std::string& message) : message(message){};
};
