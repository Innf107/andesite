#pragma once
#include "Parser/ParseError.h"
#include "Parser.h"
#include "Runtime/InstructionContext.h"
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <stack>

class Runtime
{
public:
    void processCommand(const std::string& command);

    void runContext(const InstructionContext& context);

    void addScoreboard(); // TODO
    void setScore(const unsigned int target, int score);
    void getScore(const unsigned int target);

    InstructionContext parseInstructions(const std::string& command);

    unsigned int mkTarget(const std::string& objective, const std::string& player);
    
    unsigned int addFunction(const std::string& name, const InstructionContext& context);
    unsigned int getFunction(const std::string& name);

    int returnCode = 0;
    int errorCode = 0;

    std::string lastErrorMessage;

private:
    std::vector<InstructionContext> functions;
    std::vector<int> scores;

    std::map<std::pair<std::string, std::string>, unsigned int> targetNames;
    std::map<std::string, unsigned int> functionNames;
};
