#pragma once
#include <string>

enum BytecodeOp : unsigned int {
    addScoreboardOp,       
    setScoreOp,           
    //setScoreSelectorOp,
    addScoreConstOp,
    addScoreOp,
    subScoreOp,
    mulScoreOp,
    //addScoreSelectorOp,
    getScoreOp,         
    callOp,               
};

class Bytecode {
    public:
        BytecodeOp op;
        unsigned int arguments[2];

        std::string toString();
};

Bytecode mkAddScoreboard(std::string& name);

Bytecode mkSetScore(unsigned int target, int score);

Bytecode mkAddScoreConst(unsigned int target, int score);

Bytecode mkAddScore(unsigned int target1, unsigned int target2);

Bytecode mkSubScore(unsigned int target1, unsigned int target2);

Bytecode mkMulScore(unsigned int target1, unsigned int target2);

Bytecode mkGetScore(unsigned int target);

Bytecode mkCall(unsigned int function);
