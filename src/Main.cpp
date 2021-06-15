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

int main(){
    Runtime mainRuntime;
    for (string line; prompt(line);){
        processCommand(mainRuntime, line);
    }
    return 0;
}