#ifndef _PARSE_RESULT_
#define _PARSE_RESULT_

#include <string>
#include <variant>

enum GrammarType {
    LitGrammar
,   IntGrammar
,   NameGrammar
};

class InvalidParseResult {};

class ParseIntResult {
    public:
        int parsedInt;
};

class ParseLitResult {};

class ParseNameResult {
    public:
        std::string name;
};

using ParseResult = std::variant<InvalidParseResult, ParseLitResult, ParseIntResult, ParseNameResult>;


#endif