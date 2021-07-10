#pragma once
#include "Runtime/Response.h"
#include "Runtime/Scoreboard.h"
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include "Parser.h"
#include "DatapackLoader.h"
#include "Config.h"
#include "Datapack.h"
#include <vector>
#include <map>
#include <string>

// Runtime is a class, so multiple environments can be run
// as multiple Runtimes (possibly even in parallel)
class Runtime {
    public:
        Runtime(Config&);
        Response runCommand(std::vector<std::string> command);
        Config& config;
        std::map<std::string, Datapack> datapacks;

        Parser parser;
    private:
        Response scoreboardObjectivesAdd(const std::vector<ParseResult>& args);
        Response scoreboardObjectivesAddName(const std::vector<ParseResult>& args);
        Response scoreboardObjectivesList();
        Response scoreboardObjectivesRemove(const std::vector<ParseResult>& args);

        Response scoreboardPlayersSet(const std::vector<ParseResult>& args);
        Response scoreboardPlayersGet(const std::vector<ParseResult>& args);
        Response scoreboardPlayersAdd(const std::vector<ParseResult>& args);
        Response scoreboardPlayersRemove(const std::vector<ParseResult>& args);
        Response scoreboardPlayersReset(const std::vector<ParseResult>& args);
        Response scoreboardPlayersOperation(const std::vector<ParseResult>& args);

        template<typename a>
        a parseError(std::string message, std::string received, std::string expected) {
            throw ParseError(message, received, expected);
        }

        Scoreboard scoreboard;    
};