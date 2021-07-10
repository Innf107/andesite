#include <iostream>
#include <string>
#include "Parser.h"
#include "Parser/ParseError.h"
#include "Runtime.h"
#include "Config.h"

using namespace std;

void processCommand(Runtime& runtime, const string& line){
    try {
        vector<string> segments = runtime.lexer.splitCommand(line);
        if (segments.empty())
            return;
        auto resp = runtime.runCommand(segments);
        cout << resp.message << endl;
    } catch (ParseError& p){
        cout << "PARSE ERROR: " << p.message << endl
             << "    expected: " << p.expected << endl
             << "    received: " << p.received << endl; 
        if(runtime.config.terminateOnError){
            exit(1);
        }
    }
}

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

    Runtime mainRuntime(config);
    if(args.size() == 1){
        config.terminateOnError = true;
        ifstream fileStream;
        fileStream.open(args[0]);
        if (!fileStream.is_open()){
            cout << "Could not open file '" << args[0] << "'" << endl;
            return 1;
        }
        for (string line; getline(fileStream, line);){
            processCommand(mainRuntime, line);
        }
    }
    else if(args.size() == 0){
        config.terminateOnError = false;
        for (string line; prompt(line);){
            processCommand(mainRuntime, line);
        }
    }
    else {
        cout << "Too many arguments" << endl;
        return 1;
    }
    return 0;
}