#ifndef ANDESITE_PARSE_ERROR
#define ANDESITE_PARSE_ERROR
#include <exception>
#include <string>

class ParseError : public std::exception {
public: 
    explicit ParseError(std::string message) : 
        message(message){}

    std::string message;

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif