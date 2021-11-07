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


int main(){
    Runtime mainRuntime;

    for (string line; prompt(line);){
        mainRuntime.processCommand(line);
        if (mainRuntime.errorCode != 0){
            cout << "\e[1;31m" << mainRuntime.lastErrorMessage << "\e[0m" << endl;
        } else {
            cout << mainRuntime.returnCode << endl;
        }
    }
    return 0;
}
