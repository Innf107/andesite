#pragma once
#include <string>
#include <variant>

class ParseIntResult {
    public:
        int parsedInt;
};

class ParseLitResult {};

class ParseCriteriaResult {
    public:
        std::string criteria;
};

class ParseNameResult {
    public:
        std::string name;
};


class ParseOperatorResult {
    public:
        enum Operator {
            mod,
            mul,
            add,
            sub,
            div,
            min,
            assign,
            max,
            swap
        };

        Operator op;
        
};

using ParseResult = std::variant<ParseLitResult, ParseIntResult, ParseCriteriaResult, ParseNameResult, ParseOperatorResult>;
