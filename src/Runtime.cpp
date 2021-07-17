#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include "LoaderError.h"
#include "Util.h"
#include <optional>
#include <variant>
#include <sstream>
#include <filesystem>
#define UNUSED(x) (void)(x)

using namespace std;

using Util::bind;

using RParser = Parser<Response>;

void Runtime::processCommand(const std::string& line){
    try {
        vector<string> segments = lexer.splitCommand(line);
        if (segments.empty())
            return;
        auto resp = runCommand(segments);
        cout << resp.message << endl;
    } catch (ParseError& p){
        cout << "PARSE ERROR: " << p.message << endl
             << "    expected: " << p.expected << endl
             << "    received: " << p.received << endl; 
        if(config.terminateOnError){
            exit(1);
        }
    }

}

Runtime::Runtime(Config& config): config(config), lexer(Lexer(config)){
    parser = Parser<Response>();
    parser
        .add(RParser::literal("scoreboard")
            -> add(RParser::literal("objectives")
                -> add(RParser::literal("add")
                    -> add(RParser::name()
                        -> add(RParser::criteria()
                            -> add(RParser::name()
                                -> run(bind(this, &Runtime::scoreboardObjectivesAddName)))
                            -> run(bind(this, &Runtime::scoreboardObjectivesAdd)))))
                -> add(RParser::literal("list")
                    -> run(bind(this, &Runtime::scoreboardObjectivesList)))
                -> add(RParser::literal("remove")
                    -> add(RParser::name()
                        -> run(bind(this, &Runtime::scoreboardObjectivesRemove))))
                )
            -> add(RParser::literal("players")
                -> add(RParser::literal("set")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> add(RParser::integer()
                                -> run(bind(this, &Runtime::scoreboardPlayersSet))))))
                
                -> add(RParser::literal("get")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> run(bind(this, &Runtime::scoreboardPlayersGet)))))
                -> add(RParser::literal("add")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> add(RParser::integer()
                                -> run(bind(this, &Runtime::scoreboardPlayersAdd))))))
                -> add(RParser::literal("remove")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> add(RParser::integer()
                                -> run(bind(this, &Runtime::scoreboardPlayersRemove))))))
                -> add(RParser::literal("reset")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> run(bind(this, &Runtime::scoreboardPlayersReset)))))
                -> add(RParser::literal("operation")
                    -> add(RParser::name()
                        -> add(RParser::name()
                            -> add(RParser::operation()
                                -> add(RParser::name()
                                    -> add(RParser::name()
                                        -> run(bind(this, &Runtime::scoreboardPlayersOperation))))))))
            )
        );
}


Response Runtime::runCommand(const vector<string>& cmdWithArgs){
    return parser.execute(cmdWithArgs);    
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
    UNUSED(args);
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

Response Runtime::scoreboardPlayersRemove(const vector<ParseResult>& args){
    auto& targetName    = get<ParseNameResult>(args[0]).name;
    auto& objectiveName = get<ParseNameResult>(args[1]).name;
    auto& removedScore  = get<ParseIntResult>(args[2]).parsedInt;

    auto* objective = scoreboard.getObjective(objectiveName);

    if(!objective){
        ostringstream message;
        message << "Unknown scoreboard objective '" << objectiveName << "'";
        return {0, 0, message.str()};
    }

    auto target = Target(targetName);

    auto score = objective->getScore(target);

    objective->setScore(target, score - removedScore);

    ostringstream message;
    message << "Removed " << removedScore << " from [" << objective->displayName << "] for " << target.renderName() << " (now " << score - removedScore << ")";
    return {1, score, message.str()};
}

Response Runtime::scoreboardPlayersReset(const vector<ParseResult>& args){
    auto& targetName    = get<ParseNameResult>(args[0]).name;
    auto& objectiveName = get<ParseNameResult>(args[1]).name;

    auto* objective = scoreboard.getObjective(objectiveName);

    if(!objective){
        ostringstream message;
        message << "Unknown scoreboard objective '" << objectiveName << "'";
        return {0, 0, message.str()};
    }

    auto target = Target(targetName);
    objective->resetScore(target);

    ostringstream message;
    message << "Reset [" << objective->displayName << "] for " << target.renderName();
    return {1, 1, message.str()};
}

Response Runtime::scoreboardPlayersOperation(const vector<ParseResult>& args){
    auto& target1Name    = get<ParseNameResult>(args[0]).name;
    auto& objective1Name = get<ParseNameResult>(args[1]).name;
    auto  op             = get<ParseOperatorResult>(args[2]).op;
    auto& target2Name    = get<ParseNameResult>(args[3]).name;
    auto& objective2Name = get<ParseNameResult>(args[4]).name;

    auto* objective1 = scoreboard.getObjective(objective1Name);
    if(objective1 == nullptr) return invalidObjective(objective1Name);

    auto* objective2 = scoreboard.getObjective(objective2Name);
    if(objective2 == nullptr) return invalidObjective(objective2Name);

    auto target1 = Target(target1Name);
    auto target2 = Target(target2Name);

    auto target2Score = objective2->getScore(target2);
    int result;
    if(op == ParseOperatorResult::assign){
        result = target2Score;
    }
    else {
        auto target1Score = objective1->getScore(target1);
        switch(op){
            case ParseOperatorResult::mod:
                result = target1Score % target2Score;
                break;
            case ParseOperatorResult::mul:
                result = target1Score * target2Score;
                break;
            case ParseOperatorResult::add:
                result = target1Score + target2Score;
                break;
            case ParseOperatorResult::sub:
                result = target1Score - target2Score;
                break;
            case ParseOperatorResult::div:
                result = target1Score / target2Score;
                break;
            case ParseOperatorResult::min:
                result = min(target1Score, target2Score);
                break;
            case ParseOperatorResult::max:
                result = max(target1Score, target2Score);
                break;
            case ParseOperatorResult::swap:
                objective2->setScore(target2, target1Score);
                result = target2Score;
                break;
            default:
                throw ParseError("Unhandled ParseOperation", "TODO", "TODO");
        }
    }
    objective1->setScore(target1, result);

    ostringstream message;
    message << "Set [" << objective1->displayName << "] for " << target1.renderName() << " to " << result;
    return {1, result, message.str()};
}



Response Runtime::invalidObjective(const string& objectiveName){
    ostringstream message;
    message << "Unknown scoreboard objective '" << objectiveName << "'";
    return {0, 0, message.str()};
}

void Runtime::loadFile(const filesystem::path& filepath){
    if(filepath.extension() == ".mcfunction"){
        runScript(filepath);
    } else if (filepath.extension() == ".zip"){
        ostringstream message;
        message << "Could not load " << filepath << ". Zip Datapacks are not yet supported";
        throw LoaderError (message.str());
    } else if (filesystem::is_directory(filepath)){
        ostringstream message;
        message << "Could not load " << filepath << ". Directory Datapacks are not yet supported";
        throw LoaderError (message.str());
    } else {
        ostringstream message;
        message << "Not a valid datapack or mcfunction script: " << filepath;
        throw LoaderError(message.str());
    }
}

void Runtime::runScript(const filesystem::path& file){
    ifstream fileStream;
    fileStream.open(file);
    if (!fileStream.is_open()){
        cout << "Could not open file '" << file << "'" << endl;
        throw "todo";
    }
    for (string line; getline(fileStream, line);){
        processCommand(line);
    }
}

void Runtime::warn(const string& message){
    cerr << "[WARNING]: " << message << endl;
}
