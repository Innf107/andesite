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
    Runtime();

    void processCommand(const std::string& command);

    void runContext(const InstructionContext& context);

    void addScoreboard(); // TODO
    void setScore(const unsigned int target, int score);
    void addScore(const unsigned int target, int score);
    void getScore(const unsigned int target);

    InstructionContext parseInstructions(const std::string& command);

    unsigned int mkTarget(const std::string& player, const std::string& objective);
    
    unsigned int addFunction(const std::string& name, const InstructionContext& context);
    unsigned int getFunction(const std::string& name);

    int returnCode = 0;
    int errorCode = 0;

    std::string lastErrorMessage;

    bool printBytecode = false;
    bool printScores = false;

    std::vector<int> getAllScores();

private:
    std::vector<int> scores;
    std::vector<InstructionContext> functions;

    std::map<std::pair<std::string, std::string>, unsigned int> targetNames;
    std::map<std::string, unsigned int> functionNames;

    Parser<Bytecode> bytecodeParser;
};
