// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#include <algorithm>
#include <iostream>
#include <vector>   
#include <getopt.h>
#include "readInput.hpp"
#include "routing.hpp"
using namespace std;

// These modes represent the different ways to process the input data.
// There are three modes and the state where no mode has been selected.
enum class Output {
    kNone = 0,
    kMap,
    kList,
};  

enum class Routing {
    kNone = 0,
    kStack,
    kQueue,
};

struct Options {
    Output output = Output::kMap;      
    Routing routing = Routing::kNone;  
};  

void printHelp(char *command) {
    cout << "Usage: " << command << " --stack|--queue -o M|L <INPUT_FILE>\n";
    cout << "  -s, --stack    Use a stack (DFS) for routing\n";
    cout << "  -q, --queue    Use a queue (BFS) for routing\n";
    cout << "  -o, --output   Output format (M for Map, L for List)\n";
    cout << "  -h, --help     Show this help message\n" << flush;
} 


void getOptions(int argc, char **argv, Options &options) {
    opterr = static_cast<int>(false);  
    int choice = 0;
    int index = 0;

    // NOLINTBEGIN: getopt predates C++ style, this usage is from `man getopt`
    option longOptions[] = {
  // TODO: Insert two lines for the "mode" ('m') and the "help" ('h') options.
        {"output", required_argument, nullptr, 'o'},
        {"stack", no_argument, nullptr, 's'},
        {"queue", no_argument, nullptr, 'q'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'},
    };  
    
    while ((choice = getopt_long(argc, argv, "o:hsq", 
        static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'o': {  
            string arg { optarg };
            if (arg != "M" && arg != "L") {
                cerr << "Unknown command line option\n" << flush;
                exit(1);
            }

            if (arg == "M") {
                options.output = Output::kMap;
            } else {
                options.output = Output::kList;
            } 
            break;
        } 

        case 's': {
            if (options.routing != Routing::kNone) {
                cerr << "Stack or queue can only be specified once\n";;
                exit(1);
            }
            options.routing = Routing::kStack;
            break;
        }

        case 'q': {
            if (options.routing != Routing::kNone) {
                cerr << "Stack or queue can only be specified once\n";
                exit(1);
            }
            options.routing = Routing::kQueue;
            break;

        }

        default:
            cerr << "Unknown command line option\n" << flush;
            exit(1);

        }
    } 

    if (options.routing == Routing::kNone) {
        cerr << "Stack or queue must be specified\n" << flush;
        exit(1);
    }
} 

int main(int argc, char *argv[]) {
    // 1. Process Command Line Arguments
    Options options;
    getOptions(argc, argv, options);

    // 2. Read the Input File
    readInput reader;
    reader.readData();

    // 3. Setup the Solver
    routing solver(reader);
    vector<PathStep> solution;

    // 4. Run the selected algorithm
    if (options.routing == Routing::kStack) {
        solution = solver.solveStack();
    } else {
        solution = solver.solveQueue();
    }

    if (solution.empty()) {
    return 0;
}

    // 5. Output Results 
    if (options.output == Output::kMap) {
        solver.printMapOutput(solution);
    } else if (options.output == Output::kList) {
        solver.printListOutput(solution);
    } else {
        cerr << "Error: Unknown output mode.\n";
        exit(1);
    }


    return 0;
}
