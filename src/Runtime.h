#include "Runtime/Response.h"
#include "Runtime/Scoreboard.h"
#include "Parser/ParseResult.h"
#include "Parser/ParseError.h"
#include <vector>
#include <string>

// Runtime is a class, so multiple environments can be run
// as multiple Runtimes (possibly even in parallel)
class Runtime {
    public:
        Response runCommand(std::vector<std::string> command);
    private:
        Response addScoreboard(std::vector<ParseResult> args);
        Response listScoreboard(std::vector<ParseResult> args);

        template<typename a>
        a parseError(std::string message, std::string received, std::string expected) {
            throw ParseError(message, received, expected);
        }

        Scoreboard scoreboard;
        
        std::string implementedCommands = "scoreboard";
        
};