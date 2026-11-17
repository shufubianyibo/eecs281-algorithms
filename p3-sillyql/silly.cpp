// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797926254D
#include <iostream>
#include <vector> 
#include <queue>
#include <algorithm>
#include <getopt.h>
#include "command.hpp"
using namespace std;

void printHelp(char *argv) {
    cout << "Usage: " << argv << " --help --quiet\n";
    cout << "  --help    Optional. Show this help message.\n";
    cout << "  --quiet   Optional. Print only numerical summaries.\n";
}

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    cout << boolalpha;   
    cin  >> boolalpha; 

    bool quiet = false;
    opterr = false;
    int choice, index;

    option longOptions[] = {
        {"help",  no_argument, nullptr, 'h'},
        {"quiet", no_argument, nullptr, 'q'},
        {nullptr, 0,           nullptr, '\0'},
    };

    while ((choice = getopt_long(argc, argv, "hq",
            longOptions, &index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv[0]);
                exit(0);
            case 'q':
                quiet = true;
                break;         
            default:
                cerr << "Unknown command line option\n";
                exit(1);
        }
    }

    command db(quiet);
    string cmd;
    string dummy;

    while (cout << "% " && cin >> cmd) {
        if (cmd == "QUIT") {
            db.quit();
            return 0;           
        } else if (cmd == "CREATE") {
            db.create(cin);
        } else if (cmd == "INSERT") {
            db.insert(cin);
        } else if (cmd == "REMOVE") {
            string table_name;
            cin >> table_name;
            db.remove(table_name);
        } else if (cmd == "PRINT") {
            db.print(cin);
        } else if (cmd == "DELETE") {
            db.dele(cin);
        } else if (cmd == "JOIN") {
            db.join(cin);
        } else if (cmd == "GENERATE") {
            db.generate(cin);
        } else if (cmd[0] == '#') {
            getline(cin, dummy);  
        } else {
            cout << "Error: unrecognized command\n";
            getline(cin, dummy);  
        }
    }

    return 0;
}