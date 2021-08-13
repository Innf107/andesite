#pragma once
#include <string>
#include <map>
#include "Datapack/Function.h"
#include <filesystem>
#include <zip.h>

class Datapack {
public:
    Datapack(const std::string& name, const std::vector<Function>& functions);

    void merge(Datapack other){
        for (auto& [nspace, fs] : other.namespaces){
            namespaces[nspace].merge(fs);
        }
    }

private:
    std::string name;
    std::map<std::string, std::map<std::string, Function>> namespaces;
};
