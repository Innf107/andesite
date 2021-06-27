#pragma once
#include <string>
#include <vector>
#include <exception>

class DatapackLoader {
public:
    virtual std::string readFile(const std::string& path) const = 0;
    virtual std::vector<std::string> getEntries(const std::string& path) const = 0;

    virtual ~DatapackLoader() noexcept(false) {};
};

