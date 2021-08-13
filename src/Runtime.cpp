#include "Runtime.h"
#include "Parser/ParseError.h"
#include "Parser/ParseResult.h"
#include "Parser.h"
#include "LoaderError.h"
#include "RuntimeError.h"
#include "Util.h"
#include <optional>
#include <variant>
#include <sstream>
#include <functional>
#include <filesystem>
#include <algorithm>
#include <string>
#include "Function.h"
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
            exit(2);
        }
    } catch (RuntimeError& re){
        cout << "RUNTIME ERROR: " << re.message << endl;

        if(config.terminateOnError){
            exit(3);
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
        return {0, 0, failOnStrict("An objective already exists by that name")};
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
        return {0, 0, failOnStrict("An objective already exists by that name")};
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
        return {0, 0, failOnStrict(message.str())};
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
        return {0, 0, failOnStrict(message.str())};
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
        return {0, 0, failOnStrict(message.str())};
    }

    auto target = Target(targetName);

    if (objective->hasScore(target)){
        auto score = objective->getScore(target).value_or(0);
        ostringstream message;
        message << target.renderName() << " has " << score << " [" << objective->displayName << "]";
        return {1, score, message.str()};
    }
    else {
        ostringstream message;
        message << "Can't get value of " << objective->name << " for " << targetName << "; none is set";
        return {0, 0, failOnStrict(message.str())};
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
        return {0, 0, failOnStrict(message.str())};
    }

    auto target = Target(targetName);

    auto score = objective->getScore(target).value_or(0);

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
        return {0, 0, failOnStrict(message.str())};
    }

    auto target = Target(targetName);

    auto score = objective->getScore(target).value_or(0);

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
        return {0, 0, failOnStrict(message.str())};
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

    if(config.hasXStrictScoreboard && !objective2->hasScore(target2)){
        throw RuntimeError(Util::message([&](auto& x){x << "'" << target2Name << "' does not have a score for '" << objective2Name << "'";}));
    }

    auto target2Score = objective2->getScore(target2).value_or(0);

    int result;
    if(op == ParseOperatorResult::assign){
        result = target2Score;
    }
    else {
        auto target1Score = objective1->getScore(target1).value_or(0);
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
    return {0, 0, failOnStrict(message.str())};
}

void Runtime::loadFile(const filesystem::path& filepath){
    if(filepath.extension() == ".mcfunction"){
        runScript(filepath);
    } else if (filepath.extension() == ".zip"){
        ostringstream message;
        message << "Could not load " << filepath << ". Zip Datapacks are not yet supported";
        loadDatapack(loadFromZip(filepath));
    } else if (filesystem::is_directory(filepath)){
        ostringstream message;
        message << "Could not load " << filepath << ". Directory Datapacks are not yet supported";
        loadDatapack(loadFromDirectory(filepath));
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

// Naively, all datapacks could just be kept as a list, and then every
// function call would have to iterate through every single datapack.
// To make this more efficient however, loadDatapack should really merge
// 'datapack's namespace map with a global one in the runtime.
// The datapack will also have to be stored in a list, such that
// it is possible to unload datapacks (probably just by unloading everything
// and loading all active packs.)
void Runtime::loadDatapack(const Datapack& datapack){
    loadedDatapacks.push_back(datapack);
    combinedDatapack.merge(datapack);
}

Datapack Runtime::loadFromZip(const filesystem::path& path){
    int error = ZIP_ER_OK;
    zip_t* zip = zip_open(path.c_str(), ZIP_RDONLY, &error);

    if(error != ZIP_ER_OK){
        throw LoaderError(Util::message([&](auto& x){x<<"Could not open zip file '" << path << "': libzip error code: " << error;}));
    }

    auto functions = readFunctions(zip);

    return Datapack(path.filename(), functions);
}

Datapack Runtime::loadFromDirectory(const filesystem::path& path){
    throw "LoadFromDirectory NYI";
}

vector<Function> Runtime::readFunctions(zip_t* zip){
    auto length = zip_get_num_entries(zip, 0);
    vector<Function> functions;
    for(auto i = 0; i < length; i++){
        const char* name = zip_get_name(zip, i, 0);
        
        auto dataPath = filesystem::path(name).lexically_proximate("data/");

        string nspace = *dataPath.begin(); // TODO: :/

        if (nspace != ".."){            
            auto path = dataPath.lexically_proximate(nspace + "/functions");

            if (*path.begin() != "..") // TODO: :/ 
            {
                auto contents = readZipStringIndex(zip, i);

                auto function = parseFunction(nspace, path, contents);

                functions.push_back(function);
            }
        }
    }
    return functions;
}


string Runtime::readZipStringIndex(zip_t* zip, const zip_uint64_t ix){
    zip_stat_t stat;
    zip_stat_init(&stat);
    int error = zip_stat_index(zip, ix, 0, &stat);
    if (error != ZIP_ER_OK){
        throw LoaderError(Util::message([&](auto& x){x<<"Cannot stat zip file entry with index " << ix << ": libzip error code: " << error;}));
    }
    auto* file = zip_fopen_index(zip, ix, 0);
    
    char* buffer = new char[stat.size];
    zip_fread(file, buffer, stat.size);

    error = zip_fclose(file);
    if (error != ZIP_ER_OK){
        throw LoaderError(Util::message([&](auto& x){x<<"Cannot close zip file entry with index " << ix << ": libzip error code: " << error;}));
    }

    return string(buffer);
}

Function Runtime::parseFunction(const std::string& nspace, const std::string& path, const std::string& contents){
    vector<vector<string>> lines;
    istringstream contentStream(contents);
    string line;

    while(getline(contentStream, line)){
        lines.push_back(lexer.splitCommand(line));
    }
    return Function(nspace, path, lines);
}

void Runtime::warn(const string& message){
    cerr << "[WARNING]: " << message << endl;
}

std::string Runtime::failOnStrict(const std::string& message){
    if(config.hasXStrictScoreboard){
        throw RuntimeError(message);
    } else {
        return message;
    }
}