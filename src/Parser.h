#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <fstream>
#include <iostream>
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include "Config.h"

class Parser {
public:
    Parser(Config& config);

    std::vector<std::string> splitCommand(const std::string& cmd);
    
    template <typename r>
    std::optional<r> runOnGrammar(const std::string& grammar, std::vector<std::string> cmd, std::function<r (std::vector<ParseResult>)> continuation)
    {
        auto grammarSegments = Parser::splitCommand(grammar);

        if(grammarSegments.size() != cmd.size())
            return {};

        std::vector<ParseResult> arguments;
        for (size_t i = 0; i < grammarSegments.size(); i++) {
            auto& grammarSegment = grammarSegments[i];
            auto& commandSegment = cmd[i];

            auto res = Parser::parseSegment(grammarSegment, commandSegment);

            if(std::holds_alternative<InvalidParseResult>(res)){
                return {};
            } 

            else if (!Parser::resultIgnored(res)){
                arguments.push_back(res);
            }
        }

        return continuation(arguments);
    }


    ParseResult parseSegment(const std::string& grammar, const std::string& command);
    bool resultIgnored(const ParseResult& res);

    Config& config;
private:
    bool isSpace(const char c);
};
