#ifndef _PARSE_RESULT_
#define _PARSE_RESULT_

#include <string>
#include <variant>

enum GrammarType {
    LitGrammar
,   IntGrammar
,   CriteriaGrammar
,   NameGrammar
,   OperatorGrammar
};

class InvalidParseResult {};

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

using ParseResult = std::variant<InvalidParseResult, ParseLitResult, ParseIntResult, ParseCriteriaResult, ParseNameResult, ParseOperatorResult>;


#endif