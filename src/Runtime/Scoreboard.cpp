#include "Scoreboard.h"
#include <iostream>

using namespace std;

Objective* Scoreboard::addObjective(const string& name, const string& criteria, const string& displayName){
    auto* obj = new Objective(name, criteria, displayName);

    objectives[name] = obj;

    return objectives[name];
}

Objective* Scoreboard::getObjective(const string& name){
    if (objectives.contains(name)){
        return objectives[name];
    } else {
        return nullptr;
    }
}

bool Scoreboard::removeObjective(const string& name){
    auto* obj = getObjective(name);
    if(!obj){
        return false;
    }
    delete obj;

    objectives.erase(name);
    return true;
}

vector<Objective*> Scoreboard::getAllObjectives(){
    vector<Objective*> res;
    for (auto [k, v] : objectives){
        res.push_back(v);
    }
    return res;
}

size_t Scoreboard::getObjectiveCount(){
    return objectives.size();
}
