#include <map>
#include "Target.h"

class Objective {
    public:
        std::string name;
        std::string displayName;
        std::string criteria;
        Objective(const std::string name, const std::string criteria, const std::string displayName);
        ~Objective();

        void setScore(const Target& target, const int score);
        int getScore(const Target& target);
        bool hasScore(const Target& target);
        void resetScore(const Target& target);
    private:
        std::map<Target, int> scores;
};
