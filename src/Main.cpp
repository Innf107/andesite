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
    for (auto& arg : argsAndConfig){
        if(arg == "-XRelaxedWhitespace"){
            config.hasXRelaxedWhitespace = true;
        } 
        else if (arg == "-XStrictScoreboard"){
            config.hasXStrictScoreboard = true;
        }
        else if (arg == "--terminate-on-error"){
            config.terminateOnError = true;
        }
        else if (arg.starts_with('-')){
            cerr << "Invalid flag " << arg << endl;
            exit(1);
        }
        else {
            args.push_back(arg);
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
