#ifndef _PARSE_RESULT_
#define _PARSE_RESULT_

#include <string>
#include <variant>

enum GrammarType {
    LitGrammar
,   IntGrammar
,   CriteriaGrammar
,   NameGrammar
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

using ParseResult = std::variant<InvalidParseResult, ParseLitResult, ParseIntResult, ParseCriteriaResult, ParseNameResult>;


#endif