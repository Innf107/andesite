#include "Parser.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include "Parser/ParseError.h"

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

ParseResult Parser::parseSegment(string& grammar, string& command){
    switch (Parser::getGrammarType(grammar)) {
        case LitGrammar:
            return command == grammar ? (ParseResult)ParseLitResult() : (ParseResult)InvalidParseResult();
        default:
            throw ParseError("Invalid or unsupported GrammarType", grammar, Parser::grammarTypes());
    };
}

bool Parser::resultIgnored(ParseResult& res){
    if (holds_alternative<ParseNameResult>(res))
        return false;
    return true;
}

GrammarType Parser::getGrammarType(string& grammar){
    if(grammar == "<INT>")
        return IntGrammar;
    if(grammar == "<NAME>")
        return NameGrammar;
    
    return LitGrammar; 
}

