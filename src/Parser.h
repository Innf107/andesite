#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <fstream>
#include <iostream>
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"

class Parser {
public:
    static std::vector<std::string> splitCommand(const std::string& cmd);
    
    template <typename r>
    static std::optional<r> runOnGrammar(const char* grammarFile, std::vector<std::string> cmd, std::function<r (std::vector<ParseResult>)> continuation)
    {
        std::ifstream grammarFileStream;
        grammarFileStream.open(grammarFile);

        if(!grammarFileStream.is_open()){
            throw ParseError("Grammar file does not exist", std::string(grammarFile), "valid path to a .grammar file");
        }

        std::string grammarContent;
        getline(grammarFileStream, grammarContent);

        grammarFileStream.close();

        auto grammarSegments = Parser::splitCommand(grammarContent);

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


    static ParseResult parseSegment(std::string& grammar, std::string& command);
    static bool resultIgnored(ParseResult& res);
private:
    static bool isSpace(char c);

    static GrammarType getGrammarType(std::string& grammar);

    static std::string grammarTypes(){
        return "literal, <INT>, <CRITERIA> or <NAME>";
    }
};
