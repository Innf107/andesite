#pragma once
#include <string>

enum BytecodeOp : unsigned int {
    addScoreboardOp,       
    setScoreOp,           
    //setScoreSelectorOp,
    addScoreConstOp,
    addScoreOp,
    //addScoreSelectorOp,
    subScoreConstOp,
    subScoreOp,
    mulScoreOp,
    divScoreOp,
    modScoreOp,
    minScoreOp,
    maxScoreOp,
    swapScoreOp,
    moveScoreOp,
    getScoreOp,         
    callOp,               
};

class Bytecode {
    public:
        BytecodeOp op;
        unsigned int arguments[2];

        std::string toString();
};

Bytecode mkBytecode(BytecodeOp op);
Bytecode mkBytecode(BytecodeOp op, unsigned int arg);
Bytecode mkBytecode(BytecodeOp op, unsigned int arg0, unsigned int arg1);
