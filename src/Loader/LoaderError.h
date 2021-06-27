#pragma once
#include <exception>
#include <string>

class LoaderError : public std::exception {
public: 
    explicit LoaderError(std::string message) : message(message){}

    std::string message;
};
