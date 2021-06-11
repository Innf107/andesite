#include <string>
#include <map>

class Objective; // Forward reference to avoid cyclical dependencies

class Target {
    private:
        std::string name; 
        std::map<Objective*, int> scores; 
};