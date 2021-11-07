#include "Runtime.h"
#include "Bytecode.h"
#include "Runtime/IllegalInstructionError.h"
#include "Runtime/UnboundFunctionError.h"

using namespace std;


void Runtime::processCommand(const string& command){
    InstructionContext context = parseInstructions(command);
    context.name = "<interactive>";
    runContext(context);
}

InstructionContext Runtime::parseInstructions(const string& command){
    Bytecode* bytecode = new Bytecode[]{
        mkSetScore(mkTarget("test", "test"), 5),
        mkGetScore(mkTarget("test", "test")),
    };
    return {bytecode, 2};
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

void Runtime::getScore(unsigned int target){
    returnCode = scores[target];
    errorCode = 0;
}

unsigned int Runtime::mkTarget(const string& objective, const string& player){
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

