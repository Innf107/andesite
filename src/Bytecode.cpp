#include "Bytecode.h"
#include <string>

using namespace std;

Bytecode mkAddScoreboard(std::string& name){
    //TODO
}

Bytecode mkSetScore(unsigned int target, int score){
    Bytecode code;
    code.op = setScoreOp;
    code.arguments[0] = target;
    code.arguments[1] = (unsigned int)score;
    return code;
}

Bytecode mkAddScore(unsigned int target, int score){
    Bytecode code;
    code.op = setScoreOp;
    code.arguments[0] = target;
    code.arguments[1] = (unsigned int)score;
    return code;
}

Bytecode mkGetScore(int target){
    Bytecode code;
    code.op = getScoreOp;
    code.arguments[0] = target;
    return code;
}

Bytecode mkCall(int function){
    Bytecode code;
    code.op = callOp;
    code.arguments[1] = function;
    return code;
}
