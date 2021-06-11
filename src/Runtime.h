#include "Runtime/Response.h"
#include "Runtime/Scoreboard.h"
#include <vector>
#include <string>

// Runtime is a class, so multiple environments can be run
// as multiple Runtimes (possibly even in parallel)
class Runtime {
    public:
        Response runCommand(std::vector<std::string> command);
    private:
        Response runScoreboard(std::vector<std::string> args);
        
        Scoreboard scoreboard;
        
        std::string implementedCommands = "scoreboard";
        
};