#include "Parser.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <optional>
#include "Parser/ParseError.h"

using namespace std;

Parser::Parser(Config& config): config(config) {}

bool Parser::isSpace(const char c){
    return c == ' ' || c == '\t';
}

vector<string> Parser::splitCommand(const string& cmd){
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

ParseResult Parser::parseSegment(const string& grammar, const string& command){            
    if (grammar == "<INT>")
        return (ParseResult)(ParseIntResult){stoi(command)};
    else if (grammar == "<CRITERIA>")
        return command == "dummy" ? (ParseResult)(ParseCriteriaResult){command} : (ParseResult)InvalidParseResult();
    else if (grammar == "<NAME>")
        return (ParseResult)(ParseNameResult){command};
    else if (grammar == "<OPERATION>"){
        ParseOperatorResult::Operator op;
        if (command == "%="){
            op = ParseOperatorResult::mod;
        } else if (command == "*="){
            op = ParseOperatorResult::mul;
        } else if (command == "+="){
            op = ParseOperatorResult::add;
        } else if (command == "-="){
            op = ParseOperatorResult::sub;
        } else if (command == "/=") {
            op = ParseOperatorResult::div;
        } else if (command == "<") {
            op = ParseOperatorResult::min;
        } else if (command == "="){
            op = ParseOperatorResult::assign;
        } else if (command == ">"){
            op = ParseOperatorResult::max;
        } else if (command == "><"){
            op = ParseOperatorResult::swap;
        } else {
            throw ParseError("Invalid Operation", command, "%=, *=, +=, -=, /=, <, =, > or ><");
        } 
        return (ParseResult)(ParseOperatorResult){op};
    }
    //Literal
    else {
        return command == grammar ? (ParseResult)ParseLitResult() : (ParseResult)InvalidParseResult();
    }
}

bool Parser::resultIgnored(const ParseResult& res){
    if (holds_alternative<ParseLitResult>(res))
        return true;
    return false;
}

