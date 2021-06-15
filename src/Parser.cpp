#include "Parser.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include "Parser/ParseError.h"

using namespace std;

bool Parser::isSpace(const char c){
    return c == ' ' || c == '\t';
}

vector<string> Parser::splitCommand(const string& cmd){
    vector<string> vec;
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
    return vec;
}

ParseResult Parser::parseSegment(const string& grammar, const string& command){
    switch (Parser::getGrammarType(grammar)) {
        case LitGrammar:
            return command == grammar ? (ParseResult)ParseLitResult() : (ParseResult)InvalidParseResult();
        case IntGrammar:
            return (ParseResult)(ParseIntResult){stoi(command)};
        case CriteriaGrammar:
            return command == "dummy" ? (ParseResult)(ParseCriteriaResult){command} : (ParseResult)InvalidParseResult();
        case NameGrammar:
            return (ParseResult)(ParseNameResult){command};
        default:
            throw ParseError("Invalid or unsupported GrammarType", grammar, Parser::grammarTypes());
    };
}

bool Parser::resultIgnored(const ParseResult& res){
    if (holds_alternative<ParseLitResult>(res))
        return true;
    return false;
}

GrammarType Parser::getGrammarType(const string& grammar){
    if(grammar == "<INT>")
        return IntGrammar;
    if(grammar == "<NAME>")
        return NameGrammar;
    if (grammar == "<CRITERIA>")
        return CriteriaGrammar;
    
    return LitGrammar; 
}

