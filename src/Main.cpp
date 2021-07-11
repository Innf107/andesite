#include <iostream>
#include <string>
#include "Parser.h"
#include "Parser/ParseError.h"
#include "LoaderError.h"
#include "Runtime.h"
#include "Config.h"

using namespace std;

istream& prompt(string& outstr){
    cout << "Andesite> ";
    return getline(cin, outstr);
}

pair<vector<string>, Config> parseArgsAndConfig(const vector<string>& argsAndConfig){
    vector<string> args;
    Config config;
    for (size_t i = 0; i < argsAndConfig.size(); i++){
        auto& arg = argsAndConfig[i];
        if(arg == "-XRelaxedWhitespace"){
            config.hasXRelaxedWhitespace = true;
        } 
        else if (!arg.starts_with('-')){
            args.push_back(arg);
        }
        else {
            cerr << "Invalid flag " << arg;
            exit(1);
        }
    }
    return {args, config};
}

int main(int argc, char* argv[]){
    vector<string> argsAndConfig(argv + 1, argv + argc);

    auto [args, config] = parseArgsAndConfig(argsAndConfig);

    try {
        Runtime mainRuntime(config);
        if(args.size() >= 1){
            config.terminateOnError = true;
            for (auto& arg : args){
                mainRuntime.loadFile(arg);
            }
        }
        else if(args.size() == 0){
            config.terminateOnError = false;
            for (string line; prompt(line);){
                mainRuntime.processCommand(line);
            }
        }
    }
    catch (LoaderError& err){
        cerr << "Loader Error: "<< err.message << endl;
        return 1; 
    }
    return 0;
}