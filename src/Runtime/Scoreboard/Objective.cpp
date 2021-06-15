#include "Objective.h"

using namespace std;

Objective::Objective(const std::string name, const std::string criteria, const std::string displayName) 
    : name(name), displayName(displayName), criteria(criteria) {}

Objective::~Objective(){}

void Objective::setScore(const Target& target, const int score){
    scores[target] = score;
}

int Objective::getScore(const Target& target){
    if (scores.contains(target)){
        return scores[target];
    } else {
        return 0;
    }
}

bool Objective::hasScore(const Target& target){
    return scores.contains(target);
}
