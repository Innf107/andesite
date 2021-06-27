#pragma once
#include <zip.h>
#include "DatapackLoader.h"

class ZipLoader : DatapackLoader {
public:
    ZipLoader(const std::string& zipFile);
    std::string readFile(const std::string& path) const override;
    std::vector<std::string> getEntries(const std::string& path) const override;

    ~ZipLoader() noexcept(false) override;

    std::vector<std::string> getAllEntries() const;

private:
    zip_t* zip;
};
