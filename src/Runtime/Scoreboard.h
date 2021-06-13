#include <map>
#include <string>
#include <vector>
#include "Scoreboard/Objective.h"

class Scoreboard {
    public:
        Objective* addObjective(std::string name, std::string criteria, std::string displayName);
        std::vector<Objective*> getAllObjectives();
        Objective* getObjective(std::string name);
    private:
        std::map<std::string, Objective*> objectives;       
};
