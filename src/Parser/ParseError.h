#include <exception>
#include <string>

class ParseError : public std::exception {
public: 
    explicit ParseError(std::string message, std::string received, std::string expected) : 
        message(message), received(received), expected(expected) 
    {}

    std::string message;
    std::string received;
    std::string expected;
};