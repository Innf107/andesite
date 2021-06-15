#include <map>
#include <string>
#include <vector>
#include "Scoreboard/Objective.h"

class Scoreboard {
    public:
        Objective* addObjective(const std::string& name, const std::string& criteria, const std::string& displayName);
        Objective* getObjective(const std::string& name);
        bool removeObjective(const std::string& name);

        std::vector<Objective*> getAllObjectives();
        size_t getObjectiveCount();
    
        ~Scoreboard();
    private:
        std::map<std::string, Objective*> objectives;       
};
