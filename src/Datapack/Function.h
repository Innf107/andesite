#pragma once
#include <string>
#include <vector>

class Function {
    public:
        Function(const std::string& nspace, const std::string& name, const std::vector<std::vector<std::string>>& commands)
            : nspace(nspace), name(name), commands(commands){};
        std::string nspace;
        std::string name;
        std::vector<std::vector<std::string>> commands;
};

