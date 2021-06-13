#include "Scoreboard.h"
#include <iostream>

using namespace std;

Objective* Scoreboard::addObjective(const string name, const string criteria,const string displayName){
    auto* obj = new Objective(name, criteria, displayName);

    objectives[name] = obj;

    return objectives[name];
}

vector<Objective*> Scoreboard::getAllObjectives(){
    vector<Objective*> res;
    for (auto [k, v] : objectives){
        res.push_back(v);
    }
    return res;
}

Objective* Scoreboard::getObjective(string name){
    if (objectives.contains(name)){
        return objectives[name];
    } else {
        return nullptr;
    }
}
