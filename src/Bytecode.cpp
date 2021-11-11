#include "Bytecode.h"
#include <string>
#include <sstream>

using namespace std;

string opToString(BytecodeOp op){
    switch(op){
        case addScoreboardOp: return "addScoreboard";
        case setScoreOp: return "setScore";
        case addScoreConstOp: return "addScoreConst";
        case addScoreOp: return "addScore";
        case subScoreConstOp: return "subScoreConst";
        case subScoreOp: return "subScore";
        case mulScoreOp: return "mulScore";
        case divScoreOp: return "divScore";
        case modScoreOp: return "modScore";
        case minScoreOp: return "minScore";
        case maxScoreOp: return "maxScore";
        case swapScoreOp: return "swapScore";
        case moveScoreOp: return "moveScore";
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

Bytecode mkBytecode(BytecodeOp op)
{
    Bytecode code;
    code.op = op;
    return code;
}
Bytecode mkBytecode(BytecodeOp op, unsigned int arg){
    Bytecode code;
    code.op = op;
    code.arguments[0] = arg;
    return code;
}
Bytecode mkBytecode(BytecodeOp op, unsigned int arg0, unsigned int arg1){
    Bytecode code;
    code.op = op;
    code.arguments[0] = arg0;
    code.arguments[1] = arg1;
    return code;
}
