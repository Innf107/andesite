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

    void loadFunctionFromFile(const std::filesystem::path& file);

    // `context` will not actually be mutated, but it cannot be const
    // because we need to construct a reassignable pointer to it. :/
    void runContext(InstructionContext& context);

    void addScoreboard(); // TODO
    void setScore(const unsigned int target, int score);
    void addScoreConst(const unsigned int target, int score);
    void addScore(const unsigned int target1, unsigned int target2);
    void subScoreConst(const unsigned int target, int score);
    void subScore(const unsigned int target1, unsigned int target2);
    void mulScore(const unsigned int target1, unsigned int target2);
    void divScore(const unsigned int target1, unsigned int target2);
    void modScore(const unsigned int target1, unsigned int target2);
    void minScore(const unsigned int target1, unsigned int target2);
    void maxScore(const unsigned int target1, unsigned int target2);
    void swapScore(const unsigned int target1, unsigned int target2);
    void moveScore(const unsigned int target1, unsigned int target2);
    void getScore(const unsigned int target);

    InstructionContext parseInstructions(const std::string& commands);
    InstructionContext parseInstructions(std::istream& commands);

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
