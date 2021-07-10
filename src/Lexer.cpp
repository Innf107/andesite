#include "Lexer.h"
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<string> Lexer::splitCommand(const string& cmd){
    vector<string> vec;
    if (config.hasXRelaxedWhitespace){
        string word;
        istringstream cmdStream(cmd);    
        while (cmdStream >> word){
            if(word.starts_with('#'))
                break;
            vec.push_back(word);
        }
    } 
    else {
        size_t start = 0;
        bool inWord = false;

        for (size_t i = 0; i < cmd.length() + 1; i++){
            if(cmd[i] == '#'){
                if(inWord){
                    vec.push_back(cmd.substr(start, i - start));
                }
                break;
            }
            if((isSpace(cmd[i]) || i == cmd.length()) && inWord){
                vec.push_back(cmd.substr(start, i - start));
                inWord = false;
            }
            else if (!inWord){
                inWord = true;
                start = i;
            }
        }
    }
    return vec;
}

bool Lexer::isSpace(const char c){
    return c == ' ' || c == '\t';
}
