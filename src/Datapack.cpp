#include "Datapack.h"

#include <string>
#include <map>
#include "Datapack/Function.h"
#include <filesystem>
#include <zip.h>

using namespace std;

Datapack::Datapack(const string& name, const vector<Function>& functions) : name(name) {
    for (auto& function : functions){
        // namespaces.operator[] creates a default map if the key does not exist,
        // so there is no need to manually create the inner map here.
        // For the inner map, we have to use .emplace instead, since Function does not (and should not)
        // have a default constructor. 
        namespaces[function.nspace].emplace(function.name, function);
    }
}
