#pragma once
#include <vector>
#include <string>
#include "Config.h"

class Lexer {
    public: 
        Lexer(Config& config) : config(config){}
        
        std::vector<std::string> split(const std::string& str, const char delimiter);
        std::vector<std::string> splitCommand(const std::string& cmd);
    private:
        Config& config;
        bool isSpace(const char c);
};