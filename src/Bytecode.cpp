#include "Bytecode.h"
#include <string>
#include <sstream>

using namespace std;

string opToString(BytecodeOp op){
    switch(op){
        case addScoreboardOp: return "addScoreboard";
        case setScoreOp: return "setScore";
        case addScoreOp: return "addScore";
        case getScoreOp: return "getScore";
        case callOp: return "call";
    }
    ostringstream out;
    out << op;
    return out.str();
}

string Bytecode::toString(){
    ostringstream out;
    out << opToString(op) << " " << arguments[0] << " " << arguments[1];
    return out.str();
}

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
