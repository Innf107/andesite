#include "Runtime.h"
#include "Bytecode.h"
#include "ContextContinuation.h"
#include "Runtime/IllegalInstructionError.h"
#include "Runtime/UnboundFunctionError.h"
#include "Runtime/AlreadyDefinedFunctionError.h"

using namespace std;
using namespace std::filesystem;

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
                ))
        .then(BParser::lit("function")
            .then(BParser::ident("functionName")
                .done([&](const auto& results){
                    return mkCall(getFunction(results.getIdent("functionName")));
                })));
}

void Runtime::processCommand(const string& command){
    InstructionContext context = parseInstructions(command);
    context.name = "<interactive>";
    runContext(context);
}

void Runtime::loadFunctionFromFile(const path& file){
    auto functionName = file.stem();
    if(printBytecode){
        std::cout << "bytecode in function " << functionName << ":" << endl;
    }
    ifstream fileStream(file);
    InstructionContext context = parseInstructions(fileStream);
    context.name = functionName;

    addFunction(functionName, context);
}

InstructionContext Runtime::parseInstructions(const string& commands){
    istringstream lineStream(commands);
    return parseInstructions(lineStream);
}
InstructionContext Runtime::parseInstructions(istream& commands){
    // TODO: Split by lines -> map (Split by words -> parse to bytecode)
    std::vector<Bytecode> instructions;

    for(std::string line; getline(commands, line);){
        istringstream tokenStream(line);
        std::vector<std::string> tokens;

        for(std::string token; tokenStream >> token;){
            if (token.starts_with("#")){
                break;
            }
            tokens.push_back(token);
        }

        if (!tokens.empty()){
            Bytecode instruction = bytecodeParser.run(tokens);
            if (printBytecode){
                std::cout << instruction.toString() << endl;
            }
            instructions.push_back(instruction);
        }
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

void Runtime::runContext(InstructionContext& initialContext){
    stack<ContextContinuation> callStack;
    callStack.push({&initialContext, 0});

    unsigned int instrPtr;
    while(!callStack.empty()) {
        ContextContinuation& lastContext = callStack.top();
        callStack.pop();
        InstructionContext* context = lastContext.context;
        instrPtr = lastContext.instructionPointer;

        for(; instrPtr < context->instructionCount; instrPtr++){
            Bytecode& instruction = context->instructions[instrPtr];
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
                case callOp:
                    // save current continuation on the stack
                    // instrPtr + 1, because we have to *continue* the last context instead of just repeating the call
                    callStack.push({context, instrPtr + 1});
                    // actually switch to the new context
                    context = &functions[instruction.arguments[0]];
                    // we have to set instrPtr to -1 because it is incremented at the end of the loop :/
                    instrPtr = -1;
                    //TODO: error code / return code?
                    break;
                default:
                    throw IllegalInstructionError(instruction, instrPtr, context->name);
            }
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
        throw AlreadyDefinedFunctionError(name);
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