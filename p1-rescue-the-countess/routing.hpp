// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#pragma once
#include <algorithm>
#include <vector>
#include "readInput.hpp"
#include <deque>
using namespace std;


struct PathStep {
    int room;
    int row;
    int col;
    char dir; 
};

class routing {
public:
    enum Mode { STACK, QUEUE };

    routing(readInput &input);

    std::vector<PathStep> solveStack();
    std::vector<PathStep> solveQueue();

    void printMapOutput(const std::vector<PathStep>& path);
    void printListOutput(const std::vector<PathStep>& path);

private:
    vector<char> &data;
    int rooms;
    int size;
    std::vector<int> startPosition;
    size_t discoveredCount;

    std::vector<PathStep> search(Mode mode);
    std::vector<PathStep> backtrace(vector<int> curr, 
        const vector<char> &cameFrom);
};