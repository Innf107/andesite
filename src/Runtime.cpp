#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include <optional>

using namespace std;

Response Runtime::runCommand(vector<string> cmdWithArgs){
    optional<Response> resp = 
        Parser::runOnGrammar<optional<Response>>("src/Parser/Grammar/scoreboard/objectives/add.grammar", cmdWithArgs, [&](auto args){return scoreboardObjectivesAdd(args);}).value_or(
        Parser::runOnGrammar<optional<Response>>("src/Parser/Grammar/scoreboard/objectives/add_name.grammar", cmdWithArgs, [&](auto args){return scoreboardObjectivesAddName(args);}).value_or(
        Parser::runOnGrammar<optional<Response>>("src/Parser/Grammar/scoreboard/objectives/list.grammar", cmdWithArgs, [&](auto args){return scoreboardObjectivesList(args);}).value_or(
        (optional<Response>){}
    )));
    if (resp.has_value()){
        return resp.value();
    } else {
        throw ParseError("Invalid or unsupported command", "TODO", "TODO");
    }
}

Response Runtime::scoreboardObjectivesAdd(const vector<ParseResult>& args){
    throw "Not implemented";
}

Response Runtime::scoreboardObjectivesAddName(const vector<ParseResult>& args){
    throw "Not implemented";
}

Response Runtime::scoreboardObjectivesList(const vector<ParseResult>& args){
    throw "Not implemented: list";
}

