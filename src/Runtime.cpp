#include "Runtime.h"
#include "Parser/ParseError.h"

using namespace std;

Response Runtime::runCommand(vector<string> cmdWithArgs){
    if(cmdWithArgs.empty()){
        throw ParseError("Empty Command", "", implementedCommands);
    }
    string& commandHead = cmdWithArgs.front();
    cmdWithArgs.pop_back();
    
    if (commandHead == "scoreboard"){
        return runScoreboard(cmdWithArgs);
    }
    else {
        throw ParseError("Invalid or unsupported command", commandHead, implementedCommands);
    }    
}

Response Runtime::runScoreboard(vector<string> args){
    throw "Not implemented";
}