#include <iostream>
#include <string>
#include "Parser.h"
#include "Parser/ParseError.h"
#include "Runtime.h"

using namespace std;

void processCommand(Runtime& runtime, const string& line){
    try {
        auto segments = Parser::splitCommand(line);
        auto resp = runtime.runCommand(segments);
        cout << resp.message << endl;
    } catch (ParseError& p){
        cout << "PARSE ERROR: " << p.message << endl
             << "    expected: " << p.expected << endl
             << "    received: " << p.received << endl; 
    }
}

istream& prompt(string& outstr){
    cout << "Andesite> ";
    return getline(cin, outstr);
}

int main(int argc, char* argv[]){
    vector<string> args(argv + 1, argv + argc);

    Runtime mainRuntime;
    if(args.size() == 1){
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