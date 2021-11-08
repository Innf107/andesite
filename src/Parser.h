#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Util.h"
#include "Parser/ParseError.h"

#define UNUSED(x) (void)(x)

/*
new Parser()
    .then(Parser::lit("scoreboard")
        .then(Parser::lit("players")
            .then(Parser::lit("add")
                .then(Parser::ident("player")
                    .then(Parser::integer("amount")
                        .run([](args){return test(args)})))
            .then(Parser::lit("remove")...)))))                    
*/



template<typename R> 
class Parser{
public:
    class ParseResults {
    friend Parser;
    public:
        std::string getIdent(const std::string& name) const{
            return idents.at(name);
        }
        int getInteger(const std::string& name) const{
            return integers.at(name);
        }

    private:
        std::map<std::string, std::string> idents;
        std::map<std::string, int> integers;

        void addIdent(const std::string& name, const std::string& ident){
            idents[name] = ident;
        }
        void addInteger(const std::string& name, int n){
            integers[name] = n;
        }
    };


    explicit Parser(){
        _matches = [](auto& accumulator, auto& token) -> bool{
            UNUSED(accumulator);
            UNUSED(token);
            throw ParseError("Tried to match on top level parser");
        };
    }

    R run(std::vector<std::string>& tokens){
        auto results = ParseResults();
        return runInner(results, tokens, 0);
    }

    Parser& then(const Parser p) {
        children.push_back(p);
        return *this; 
    }

    Parser& done(std::function<R (const ParseResults&)> f){
        leafRunner = f;
        isLeaf = true;
        return *this;
    }

    static Parser lit(const std::string& lit){
        return Parser([lit](const std::string& token, ParseResults& results){
            UNUSED(results);
            return token == lit;
        });
    }

    static Parser ident(const std::string& name){
        return Parser([name](auto& token, ParseResults& results){
            results.addIdent(name, token);
            return true;
        });
    }

    static Parser integer(const std::string& name){
        return Parser([name](const std::string& token, ParseResults& results) -> bool {
            std::stringstream tokenStream(token);
            int result; 
            tokenStream >> result;
            if(tokenStream.fail()){
                return false;
            }
            results.addInteger(name, result);
            return true;
        });
    }

private:
    R runInner(ParseResults& accumulated, std::vector<std::string>& tokens, int index){
        if(isLeaf && (unsigned int) index == tokens.size()){
            return leafRunner(accumulated);
        } else {
            if ((unsigned int) index >= tokens.size()){
                throw ParseError("Unexpected EOF");
            }
            for(auto& child : children){
                if(child._matches(tokens[index], accumulated)){
                    return child.runInner(accumulated, tokens, index + 1);
                }
            }
        }
        std::ostringstream message;
        message << "Unexpected: " << tokens[index];
        throw ParseError(message.str());
    }

    bool isLeaf = false;
    std::function<R (const ParseResults&)> leafRunner;

    Parser(std::function<bool (const std::string&, ParseResults&)> _matches): _matches(_matches){}

    std::function<bool (const std::string&, ParseResults&)> _matches;
    std::vector<Parser> children;
};


