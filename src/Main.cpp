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
        try {
            mainRuntime.processCommand(line);
            if (mainRuntime.errorCode != 0){
                cout << "\e[1;31m" << mainRuntime.lastErrorMessage << "\e[0m" << endl;
            } else {
                cout << mainRuntime.returnCode << endl;
                if (mainRuntime.printScores){
                    cout << "scores: [";
                    for (int score : mainRuntime.scores){
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
