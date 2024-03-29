#pragma once
#include "Runtime/Response.h"
#include "Runtime/Scoreboard.h"
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include "Parser.h"
#include "Lexer.h"
#include "Config.h"
#include "Datapack.h"
#include <vector>
#include <map>
#include <string>
#include <filesystem>

class Runtime {
    public:
        Runtime(Config&);
        void processCommand(const std::string& line);
        Response runCommand(const std::vector<std::string>& command);

        void loadFile(const std::filesystem::path& filepath);

        Config& config;
        std::map<std::string, Datapack> datapacks;

        Parser<Response> parser;
        Lexer lexer;
    private:
        Response scoreboardObjectivesAdd(const std::vector<ParseResult>& args);
        Response scoreboardObjectivesAddName(const std::vector<ParseResult>& args);
        Response scoreboardObjectivesList(const std::vector<ParseResult>& args);
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

        Datapack combinedDatapack = Datapack("GLOBAL", {});
        std::vector<Datapack> loadedDatapacks;

        Response invalidObjective(const std::string& objectiveName);

        void runScript(const std::filesystem::path& file);

        void loadDatapack(const Datapack& datapack);

        Datapack loadFromZip(const std::filesystem::path& path);
        Datapack loadFromDirectory(const std::filesystem::path& path);
        std::vector<Function> readFunctions(zip_t* zip);

        Function parseFunction(const std::string& nspace, const std::string& path, const std::string& contents);

        std::string readZipStringIndex(zip_t* zip, const zip_uint64_t ix);

        void warn(const std::string& message);

        std::string failOnStrict(const std::string& message);
};
