#include <iostream>
#include <string>
#include "Parser.h"
#include "Parser/ParseError.h"
#include "Runtime.h"

using namespace std;

istream& prompt(string& outstr){
    cout << "Andesite> ";
    return getline(cin, outstr);
}

void usage(char* progName){
    cout << "usage: " << progName << " [options] [file...]" << endl;
    cout << "options:" << endl;
    cout << "  --help | -h                      show this help text" << endl;
    cout << "  --[no-]print-bytecode            print the bytecode generated by each interactive command" << endl;
    cout << "  --[no-]print-scores              print all player scores after each interactive command" << endl;
}

int main(int argc, char* argv[]){
    Runtime mainRuntime;

    vector<string> files;

    for(int i = 1; i < argc; i++){
        string arg = argv[i];
        if(arg == "--help" || arg == "-h"){
            usage(argv[0]);
            return 0;
        }
        if(arg == "--print-bytecode"){
            mainRuntime.printBytecode = true;
        } 
        else if (arg == "--no-print-bytecode"){
            mainRuntime.printBytecode = false;
        }
        else if (arg == "--print-scores"){
            mainRuntime.printScores = true;
        }
        else if (arg == "--no-print-scores"){
            mainRuntime.printScores = false;
        }
        else if (arg.starts_with('-')){
            cerr << "Invalid flag: " << arg << endl;
            usage(argv[0]);
            return 1;
        }
        else {
            files.push_back(arg);
        }
    }

    for (string line; prompt(line);){
        try {
            mainRuntime.processCommand(line);
            if (mainRuntime.errorCode != 0){
                cout << "\e[1;31m" << mainRuntime.lastErrorMessage << "\e[0m" << endl;
            } else {
                cout << mainRuntime.returnCode << endl;
                if (mainRuntime.printScores){
                    cout << "scores: [ ";
                    for (int score : mainRuntime.getAllScores()){
                        cout << score << " ";
                    }
                    cout << "]" << endl;
                }
            }
        } catch (ParseError& error){
            cerr << "\e[1;31mParse Error: " << error.message << "\e[0m" << endl;
        }
    }
    return 0;
}
