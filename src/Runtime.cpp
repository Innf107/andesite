#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include <optional>
#include <variant>
#include <sstream>

using namespace std;

Response Runtime::runCommand(vector<string> cmdWithArgs){
    optional<Response> resp = 
        Parser::runOnGrammar<optional<Response>>("scoreboard objectives add <NAME> <CRITERIA>", cmdWithArgs, [&](auto args){return scoreboardObjectivesAdd(args);}).value_or(
        Parser::runOnGrammar<optional<Response>>("scoreboard objectives add <NAME> <CRITERIA> <NAME>", cmdWithArgs, [&](auto args){return scoreboardObjectivesAddName(args);}).value_or(
        Parser::runOnGrammar<optional<Response>>("scoreboard objectives list", cmdWithArgs, [&](auto args){return scoreboardObjectivesList(args);}).value_or(
        (optional<Response>){}
    )));
    if (resp.has_value()){
        return resp.value();
    } else {
        throw ParseError("Invalid or unsupported command", "TODO", "TODO");
    }
}

Response Runtime::scoreboardObjectivesAdd(const vector<ParseResult>& args){    
    auto name = std::get<ParseNameResult>(args[0]).name;
    auto criteria = std::get<ParseCriteriaResult>(args[1]).criteria;
    
    if (scoreboard.getObjective(name)){
        return {0, 0, "An objective already exists by that name"};
    } else {
        scoreboard.addObjective(name, criteria, name);
        std::ostringstream message;
        message << "Created new objective [" << name << "]";
        return {1, (int)scoreboard.getAllObjectives().size(), message.str()};
    }
}

Response Runtime::scoreboardObjectivesAddName(const vector<ParseResult>& args){
    auto name = std::get<ParseNameResult>(args[0]).name;
    auto criteria = std::get<ParseCriteriaResult>(args[1]).criteria;
    auto displayName = std::get<ParseNameResult>(args[2]).name;

    if (scoreboard.getObjective(name)){
        return {0, 0, "An objective already exists by that name"};
    } else {
        scoreboard.addObjective(name, criteria, name);
        std::ostringstream message;
        message << "Created new objective [" << name << "]";
        return {1, 1, message.str()};
    }
}

Response Runtime::scoreboardObjectivesList(const vector<ParseResult>& args){
    auto objectives = scoreboard.getAllObjectives();
    std::ostringstream message;

    if (objectives.size() == 0){
        message << "There are no objectives";
    } else {
        message << "There are " << objectives.size() << " objectives: [" << objectives[0]->displayName << "]";
    }
    bool skip = true;
    for (auto objective: objectives){
        if(!skip){
            message << ", [" << objective->displayName << "]";
        }
        skip = false;
    }
    return {1, (int)objectives.size(), message.str()};
}

