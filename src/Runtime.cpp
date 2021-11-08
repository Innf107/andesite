#include "Runtime.h"
#include "Bytecode.h"
#include "Runtime/IllegalInstructionError.h"
#include "Runtime/UnboundFunctionError.h"

using namespace std;

using BParser = Parser<Bytecode>;

Runtime::Runtime(){
    bytecodeParser = BParser()
        .then(BParser::lit("scoreboard")
            .then(BParser::lit("players")
                .then(BParser::lit("set")
                    .then(BParser::ident("player")
                        .then(BParser::ident("objective")
                            .then(BParser::integer("value")
                                .done([&](const BParser::ParseResults& results){
                                    return mkSetScore(mkTarget(results.getIdent("player"), results.getIdent("objective")), results.getInteger("value"));
                                })))))
                .then(BParser::lit("get")
                    .then(BParser::ident("player")
                        .then(BParser::ident("objective")
                            .done([&](const auto& results){
                                return mkGetScore(mkTarget(results.getIdent("player"), results.getIdent("objective")));
                            }))))
                .then(BParser::lit("add")
                    .then(BParser::ident("player")
                        .then(BParser::ident("objective")
                            .then(BParser::integer("value")
                                .done([&](const auto& results){
                                    return mkAddScore(mkTarget(results.getIdent("player"), results.getIdent("objective")), results.getInteger("value"));
                                })))))
                ));
}

void Runtime::processCommand(const string& command){
    InstructionContext context = parseInstructions(command);
    context.name = "<interactive>";
    runContext(context);
}

InstructionContext Runtime::parseInstructions(const string& commands){
    // TODO: Split by lines -> map (Split by words -> parse to bytecode)
    istringstream lineStream(commands);
    std::vector<Bytecode> instructions;

    for(std::string line; getline(lineStream, line);){
        istringstream tokenStream(line);
        std::vector<std::string> tokens;

        for(std::string token; tokenStream >> token;){
            tokens.push_back(token);
        }

        Bytecode instruction = bytecodeParser.run(tokens);
        if (printBytecode){
            std::cout << instruction.toString() << endl;
        }
        instructions.push_back(instruction);
    }

    InstructionContext context;

    Bytecode* instructionArray = new Bytecode[instructions.size()];
    for (unsigned int i = 0; i < instructions.size(); i++){
        instructionArray[i] = instructions[i];
    }
    context.instructions = instructionArray;
    context.instructionCount = instructions.size();

    return context;
}

void Runtime::runContext(const InstructionContext& context){
    for(unsigned int instrPtr = 0; instrPtr < context.instructionCount; instrPtr++){
        Bytecode& instruction = context.instructions[instrPtr];
        switch(instruction.op){
            case addScoreboardOp:
                //TODO
                addScoreboard();
                break;
            case setScoreOp:
                setScore(instruction.arguments[0], (int)instruction.arguments[1]);
                break;
            case addScoreOp:
                addScore(instruction.arguments[0], (int)instruction.arguments[1]);
                break;
            case getScoreOp:
                getScore(instruction.arguments[0]);
                break;
            default:
                throw IllegalInstructionError(instruction, instrPtr, context.name);
        }
    }
}

void Runtime::addScoreboard(){

}

void Runtime::setScore(unsigned int target, int score){
    scores[target] = score;

    returnCode = score;
    errorCode = 0;
}

void Runtime::addScore(unsigned int target, int score){
    scores[target] += score;

    returnCode = scores[target];
    errorCode = 0;
}

void Runtime::getScore(unsigned int target){
    returnCode = scores[target];
    errorCode = 0;
}

unsigned int Runtime::mkTarget(const string& player, const string& objective){
    const std::pair target(objective, player); 
    if (targetNames.contains(target)){
        return targetNames[target];
    } else {
        unsigned int ix = scores.size();
        scores.push_back(0);

        targetNames[target] = ix;


        return ix;
    }
}

unsigned int Runtime::addFunction(const string& name, const InstructionContext& context){
    if (functionNames.contains(name)){
        int functionIX = functionNames[name];

        functions[functionIX] = context;

        return functionIX;
    } else {
        int functionIX = functions.size();
        functions.push_back(context);

        functionNames[name] = functionIX;

        return functionIX;
    }
}

unsigned int Runtime::getFunction(const string& name){
    if (functionNames.contains(name)){
        return functionNames[name];
    } else {
        throw UnboundFunctionError(name);
    }
}

vector<int> Runtime::getAllScores(){
    return scores;
}