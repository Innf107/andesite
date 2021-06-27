#include <string>
#include <sstream>
#include <memory>
#include "ZipLoader.h"
#include "LoaderError.h"

using namespace std;
ZipLoader::ZipLoader(const string& zipPath)
{
    int error = ZIP_ER_OK;
    zip = zip_open(zipPath.c_str(), 0, &error);
    if (error != ZIP_ER_OK){
        ostringstream message;
        message << "Error opening zip file '" << zipPath << "' Error Code: " << error;
        throw LoaderError(message.str());
    }
}


string ZipLoader::readFile(const string& path) const{
    auto* file = zip_fopen(zip, path.c_str(), 0);

    unique_ptr<zip_stat_t> stat (new zip_stat_t());
    int error = zip_stat(zip, path.c_str(), 0, stat.get());
    if (error != ZIP_ER_OK){
        ostringstream message;
        message << "Error reading zip file entry at '" << path << "' Error Code: " << error;
        throw LoaderError(message.str());
    }

    char* returnBuffer = new char[stat->size];
    zip_fread(file, returnBuffer, stat->size);

    zip_fclose(file);

    string resultString(returnBuffer);
    
    delete[] returnBuffer;
    return resultString;
}

vector<string> ZipLoader::getEntries(const string& path) const {
    auto entryCount = zip_get_num_entries(zip, 0);

    vector<string> results(entryCount);

    for (int i = 0; i < entryCount; i++){        
        unique_ptr<zip_stat_t> stat (new zip_stat_t());
        int error = zip_stat_index(zip, i, 0, stat.get());
        if(error != ZIP_ER_OK) {
            ostringstream message;
            message << "PANIC: Cannot stat file at index '" << i << "' in zip while enumerating. Error Code: " << error;
            throw LoaderError(message.str());
        }
    
        string entry(stat->name);

        if (!entry.starts_with(path)){
            int pathLength = path.ends_with('/') ? path.length() : path.length() + 1;
            results.push_back(entry.substr(pathLength));
        }        
    }
    return results;
}

vector<string> ZipLoader::getAllEntries() const {
    auto entryCount = zip_get_num_entries(zip, 0);

    vector<string> results(entryCount);

    for (int i = 0; i < entryCount; i++){        
        unique_ptr<zip_stat_t> stat (new zip_stat_t());
        int error = zip_stat_index(zip, i, 0, stat.get());
        if(error != ZIP_ER_OK) {
            ostringstream message;
            message << "PANIC: Cannot stat file at index '" << i << "' in zip while enumerating. Error Code: " << error;
            throw LoaderError(message.str());
        }

        results.push_back(string(stat->name));
    }
    return results;
}

ZipLoader::~ZipLoader() noexcept(false) {
    int error = zip_close(zip);
    if (error != ZIP_ER_OK){
        free(zip);
        ostringstream message;
        message << "Could not close zip file! Error Code: " << error;
        throw LoaderError(message.str());
    }
}

