#pragma once
#include <string>
#include <map>
#include "Datapack/Function.h"

class Datapack {
public:
    std::string name;
    std::map<std::string, Function> functions;
};
