#pragma once
#include "Runtime/Response.h"
#include "Runtime/Scoreboard.h"
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include "Parser.h"
#include "Lexer.h"
#include "DatapackLoader.h"
#include "Config.h"
#include "Datapack.h"
#include <vector>
#include <map>
#include <string>

class Runtime {
    public:
        Runtime(Config&);
        Response runCommand(const std::vector<std::string>& command);
        Config& config;
        std::map<std::string, Datapack> datapacks;

        Parser<Response> parser;
        Lexer lexer;
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

        Response invalidObjective(const std::string& objectiveName);
};
