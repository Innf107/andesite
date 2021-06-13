#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include <optional>

using namespace std;

Response Runtime::runCommand(vector<string> cmdWithArgs){
    optional<Response> resp = Parser::runOnGrammar<optional<Response>>("src/Parser/Grammar/scoreboard/add.grammar", cmdWithArgs, [&](vector<ParseResult> args){return addScoreboard(args);}).value_or(
        Parser::runOnGrammar<optional<Response>>("src/Parser/Grammar/scoreboard/list.grammar", cmdWithArgs, [&](vector<ParseResult> args){return listScoreboard(args);}).value_or(
            (optional<Response>){}
        //parseError<Response>("Invalid or unsupported command", "TODO", "TODO")
    ));
    if (resp.has_value()){
        return resp.value();
    } else {
        throw ParseError("Invalid or unsupported command", "TODO", "TODO");
    }
}

Response Runtime::addScoreboard(vector<ParseResult> args){
    throw "Not implemented";
}

Response Runtime::listScoreboard(vector<ParseResult> args){
    throw "Not implemented: list";
}

