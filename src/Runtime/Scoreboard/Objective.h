#include <map>
#include "Target.h"

class Objective {
    public:
        std::string name;
        std::string displayName;
        std::string criteria;
        Objective(std::string name, std::string criteria, std::string displayName);
        ~Objective();
    private:
        std::map<Target*, int> scores;
};
