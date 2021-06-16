#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include <optional>
#include <variant>
#include <sstream>

using namespace std;

struct CommandSpec {
    std::string grammar;
    std::function<Response (const std::vector<ParseResult> &args)> cont;
};

Response Runtime::runCommand(vector<string> cmdWithArgs){
    vector<CommandSpec> commands;
    commands.push_back({"scoreboard objectives add <NAME> <CRITERIA>", [&](auto args){return scoreboardObjectivesAdd(args);}});
    commands.push_back({"scoreboard objectives add <NAME> <CRITERIA> <NAME>", [&](auto args){return scoreboardObjectivesAddName(args);}});
    commands.push_back({"scoreboard objectives list", [&](auto args){return scoreboardObjectivesList(args);}});
    commands.push_back({"scoreboard objectives remove <NAME>", [&](auto args){return scoreboardObjectivesRemove(args);}});

    commands.push_back({"scoreboard players set <NAME> <NAME> <INT>", [&](auto args){return scoreboardPlayersSet(args);}});
    commands.push_back({"scoreboard players get <NAME> <NAME>", [&](auto args){return scoreboardPlayersGet(args);}});
    commands.push_back({"scoreboard players add <NAME> <NAME> <INT>", [&](auto args){return scoreboardPlayersAdd(args);}});

    for (auto& [grammar, cont] : commands) {
        auto resp = Parser::runOnGrammar<Response>(grammar, cmdWithArgs, cont);
        if (resp.has_value())
            return resp.value();
    }
    throw ParseError("Invalid or unsupported command", "TODO", "TODO");
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
        return {1, (int)scoreboard.getObjectiveCount(), message.str()};
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
        return {1, (int)scoreboard.getObjectiveCount(), message.str()};
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

//TODO: This has to traverse 'objectives' twice to get the name.
//This might be optimized by using objectives.extract
Response Runtime::scoreboardObjectivesRemove(const vector<ParseResult>& args){
    auto& name = get<ParseNameResult>(args[0]).name;

    ostringstream message;
    auto* obj = scoreboard.getObjective(name);
    if(obj){
        message << "Removed objective [" << obj->displayName << "]";
        scoreboard.removeObjective(name);
        return {1, (int)scoreboard.getObjectiveCount(), message.str()};

    } else {
        message << "Unknown scoreboard objective '" << name << "'";
        return {0, 0, message.str()};
    }
}

Response Runtime::scoreboardPlayersSet(const vector<ParseResult>& args){
    auto& targetName    = get<ParseNameResult>(args[0]).name;
    auto& objectiveName = get<ParseNameResult>(args[1]).name;
    auto  score         = get<ParseIntResult>(args[2]).parsedInt;

    auto* objective = scoreboard.getObjective(objectiveName);

    if(!objective){
        ostringstream message;
        message << "Unknown scoreboard objective '" << objectiveName << "'";
        return {0, 0, message.str()};
    }

    auto target = Target(targetName);

    objective->setScore(targetName, score);
    ostringstream message;
    message << "Set [" << objective->displayName << "] for " << target.renderName() << " to " << score;
    return {1, score, message.str()};
}

Response Runtime::scoreboardPlayersGet(const vector<ParseResult>& args){
    auto& targetName    = get<ParseNameResult>(args[0]).name;
    auto& objectiveName = get<ParseNameResult>(args[1]).name;

    auto* objective = scoreboard.getObjective(objectiveName);

    if(!objective){
        ostringstream message;
        message << "Unknown scoreboard objective '" << objectiveName << "'";
        return {0, 0, message.str()};
    }

    auto target = Target(targetName);

    if (objective->hasScore(target)){
        auto score = objective->getScore(target);
        ostringstream message;
        message << target.renderName() << " has " << score << " [" << objective->displayName << "]";
        return {1, score, message.str()};
    }
    else {
        ostringstream message;
        message << "Can't get value of " << objective->name << " for " << targetName << "; none is set";
        return {0, 0, message.str()};
    }

}

Response Runtime::scoreboardPlayersAdd(const vector<ParseResult>& args){
    auto& targetName    = get<ParseNameResult>(args[0]).name;
    auto& objectiveName = get<ParseNameResult>(args[1]).name;
    auto& addedScore    = get<ParseIntResult>(args[2]).parsedInt;

    auto* objective = scoreboard.getObjective(objectiveName);

    if(!objective){
        ostringstream message;
        message << "Unknown scoreboard objective '" << objectiveName << "'";
        return {0, 0, message.str()};
    }

    auto target = Target(targetName);

    auto score = objective->getScore(target);

    objective->setScore(target, score + addedScore);

    ostringstream message;
    message << "Added " << addedScore << " to [" << objective->displayName << "] for " << target.renderName() << " (now " << score + addedScore << ")";
    return {1, score, message.str()};
}

