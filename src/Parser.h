#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include "Util.h"
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include "Config.h"

#define UNUSED(x) (void)(x)

template<typename R> 
class Parser{
    public:
        explicit Parser() 
        : expected("")
        , children(std::vector<Parser*>())
        , _execute([&](const auto& args, int start, auto& partial_results, auto& children){
            for(auto* c : children){
                auto result = c->_execute(args, start, partial_results, c->children);
                if(result.has_value()){
                    return result.value();
                }
            }
            throw ParseError("Invalid or unsupported command", args[0], mkExpected(children));
        }){}

        explicit Parser(const std::string& expected, std::function<std::optional<R> (const std::vector<std::string>&, int, std::vector<ParseResult>&, std::vector<Parser*>&)> exec) 
        : expected(expected) 
        , children(std::vector<Parser*>())
        , _execute(exec){}

        ~Parser(){
            for (auto* c : children){
                delete c;
            }
        }

        R execute(const std::vector<std::string>& args){
            std::vector<ParseResult> results;
            auto res = _execute(args, 0, results, children);
            return res.value();
        }
        Parser* add(Parser* child){
            children.push_back(child);
            return this;
        }
        Parser* run(std::function<R (const std::vector<ParseResult>& args)> run){
            children.push_back(new Parser("<EOF>", [run](auto& args, int start, auto& results, auto& children){
                UNUSED(args); UNUSED(start); UNUSED(children);
                return run(results);
            }));
            return this;
        }
        
        static Parser* literal(const std::string& lit){
            return argParser(lit, [=](auto& arg){return Util::when(arg == lit, ParseLitResult());});
        }
        static Parser* name(){
            return argParser("<NAME>", [](const std::string& arg){return (ParseNameResult){arg};});
        }

        static Parser* criteria(){
            return argParser("<CRITERIA>", [](const std::string& arg){return Util::when(arg == "dummy", (ParseCriteriaResult){arg});});
        }

        static Parser* integer(){
            return argParser("<INTEGER>", [](const std::string& arg) -> std::optional<ParseResult> {
                try {
                    return (ParseIntResult){stoi(arg)};
                } catch (std::invalid_argument&) {
                    return {};
                }
            });
        }

        static Parser* operation(){
            return argParser("<OPERATION>", [](const std::string& arg) -> std::optional<ParseResult> {
                ParseOperatorResult::Operator op;
                if (arg == "%="){
                    op = ParseOperatorResult::mod;
                } else if (arg == "*="){
                    op = ParseOperatorResult::mul;
                } else if (arg == "+="){
                    op = ParseOperatorResult::add;
                } else if (arg == "-="){
                    op = ParseOperatorResult::sub;
                } else if (arg == "/=") {
                    op = ParseOperatorResult::div;
                } else if (arg == "<") {
                    op = ParseOperatorResult::min;
                } else if (arg == "="){
                    op = ParseOperatorResult::assign;
                } else if (arg == ">"){
                    op = ParseOperatorResult::max;
                } else if (arg == "><"){
                    op = ParseOperatorResult::swap;
                } else {
                    return {};
                }
                return (ParseOperatorResult){op};
            });
        }

    private:
        std::string expected;
        std::vector<Parser*> children;
        std::function<std::optional<R> (const std::vector<std::string>&, int, std::vector<ParseResult>&, std::vector<Parser*>&)> _execute;

        static Parser* argParser(const std::string& expected, std::function<std::optional<ParseResult> (const std::string&)> parser){
            return new Parser(expected, [parser](const std::vector<std::string>& args, int start, std::vector<ParseResult>& partialResults, std::vector<Parser*>& children){
                if(start >= (int)args.size()){
                    return (std::optional<R>){};
                }
                auto& arg = args[start];
                auto res = parser(arg);
                if(res.has_value()){
                    if(!std::holds_alternative<ParseLitResult>(res.value())){
                        partialResults.push_back(res.value());
                    }
                    for(auto* c : children){
                        auto res = c->_execute(args, start + 1, partialResults, c->children);
                        if(res.has_value()){
                            return res;
                        }
                    }
                    auto& failedArg = (int)args.size() > start + 1 ? args[start + 1] : "<EOF>";
                    throw ParseError("Invalid or unsupported command", failedArg, mkExpected(children));
                } else {
                    return (std::optional<R>){};
                }
            });
        }

        static std::string mkExpected(const std::vector<Parser*>& children){
            std::ostringstream message;
            for (int i = 0; i < (int)children.size() - 2; i++){
                message << children[i]->expected << ", ";
            }
            if (children.size() >= 2) {
                message << children[children.size() - 2]->expected;
                message << " or ";
            }
            if (children.size() >= 1){
                message << children[children.size() - 1]->expected;
            }
            return message.str();
        }
};

