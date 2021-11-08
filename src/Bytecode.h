#pragma once
#include <string>

enum BytecodeOp : unsigned int {
    addScoreboardOp,       
    setScoreOp,           
    //setScoreSelectorOp,
    addScoreOp,           
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

Bytecode mkAddScore(unsigned int target, int score);

Bytecode mkGetScore(int target);

Bytecode mkCall(int function);
