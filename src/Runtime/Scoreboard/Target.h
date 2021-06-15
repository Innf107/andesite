#include <string>
#include <map>

class Objective; // Forward reference to avoid cyclical dependencies

class Target {
    public:
        Target(std::string name);

        bool operator<(const Target& other) const {
            return name < other.name;
        }

        std::string renderName() const {
            return name;
        }
    private:
        std::string name; 
        std::map<Objective*, int> scores; 
};