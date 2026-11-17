// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA16442622C7
#include <iostream>
#include <vector> 
#include <queue>
#include <algorithm>
#include <getopt.h>
#include <cmath>
#include <limits>
#include <iomanip>
#include <string>
#include "AmongUs.hpp"
using namespace std;

void printHelp(char *argv) {
    cout << "Usage: " << argv << " --help --mode\n";
    cout << "  --mode    Choose mode (MST, FASTTSP, OPTTSP)\n";
    cout << "  --help    Print this help message\n";
}

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);

    opterr = false;
    int choice, index;

    option longOptions[] = {
        {"mode",  required_argument, nullptr, 'm'},
        {"help",  no_argument,       nullptr, 'h'},
        {nullptr, 0,           nullptr, '\0'},
    };
    AmongUs game;
    while ((choice = getopt_long(argc, argv, "hm:",
            longOptions, &index)) != -1) {
        switch (choice) {
            case 'h':{
                printHelp(argv[0]);
                exit(0);}
            case 'm':{
            if (optarg == nullptr) {
                cerr << "Error: No mode specified\n";
                exit(1);
            }
            
            string modeArg(optarg);
            if (modeArg != "MST" && modeArg != "FASTTSP" && modeArg != "OPTTSP") {
                cerr << "Error: Invalid mode\n";
                exit(1);
            }
            game.solve(modeArg);
            break;  
        }       
            default:
                cerr << "Error: Invalid command line option\n";
                exit(1);
        }
    }


    return 0;
}