#pragma once
#include <exception>
#include <string>

class Panic : public std::exception {
public: 
    explicit Panic(std::string message) : 
        message(message){}

    std::string message;

    const char* what() const noexcept override {
        return message.c_str();
    }
};
