#include "Parser.h"
#include <string>
#include <vector>

using namespace std;

bool Parser::isSpace(char c){
    return c == ' ' || c == '\t';
}

vector<string> Parser::splitCommand(const string& cmd){
    vector<string> vec;
    size_t start = 0;
    bool inWord = false;
    for (size_t i = 0; i < cmd.length() + 1; i++){
        if((isSpace(cmd[i]) || i == cmd.length()) && inWord){
            vec.push_back(cmd.substr(start, i - start));
            inWord = false;
        }
        else if (!inWord){
            inWord = true;
            start = i;
        }
    }
    return vec;
}