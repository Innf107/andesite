#include "Objective.h"

using namespace std;

Objective::Objective(std::string name, std::string criteria, std::string displayName) 
    : name(name), displayName(displayName), criteria(criteria) {}

Objective::~Objective(){}
