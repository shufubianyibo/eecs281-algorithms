// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "routing.hpp"
#include <deque>
using namespace std;


// constructor
routing::routing(readInput &input) 
    : data(input.getData()), 
      rooms(input.getRooms()), 
      size(input.getSize()), 
      startPosition(input.getStartPosition()) 
{}
// solve using stack
vector<PathStep> routing::solveStack() {
    return search(STACK);
}
// solve using queue
vector<PathStep> routing::solveQueue() {
    return search(QUEUE);
}

// main search function using stack or queue
vector<PathStep> routing::search(Mode mode) {
    bool found = false;
    vector<int> finalPos = {-1, -1, -1};
    size_t numRooms = static_cast<size_t>(rooms);
    size_t dim = static_cast<size_t>(size);
    size_t floorSize = dim * dim;
    const long int northOffset = -static_cast<long int>(dim);
    const long int southOffset = static_cast<long int>(dim);
    const long int eastOffset  = 1;
    const long int westOffset  = -1;
    

    vector<char> cameFrom(numRooms * floorSize, 0);

    deque<size_t> s;

    size_t startIndex = (static_cast<size_t>(startPosition[0]) * floorSize) + 
                        (static_cast<size_t>(startPosition[1]) * dim) + 
                        static_cast<size_t>(startPosition[2]);
    s.push_back(static_cast<size_t>(startIndex));
    cameFrom[startIndex] = 'S';

    discoveredCount = 1; 

    while (!s.empty()) {
        size_t current;
        if (mode == STACK) {
            current = s.back();  
            s.pop_back();
        } else {
            current = s.front(); 
            s.pop_front();
        }

        // int r = current.room;
        // int x = current.row;
        // int y = current.col;
        // size_t baseRoomIdx = static_cast<size_t>(r) * floorSize;
        
        size_t currentIdx = static_cast<size_t>(current);
        size_t roomIdx = currentIdx / floorSize;
        size_t roomOffset = currentIdx % floorSize;
        char tile = data[currentIdx];
        bool isPipe = (tile >= '0' && tile <= '9');

        if (isPipe) {
            int targetRoom = tile - '0';
            int r = static_cast<int>(roomIdx);
            
            if (targetRoom < rooms && targetRoom != r) {
                size_t targetIdx = (static_cast<size_t>(targetRoom) 
                * floorSize) + roomOffset;
                if (cameFrom[targetIdx] == 0) {
                    char targetTile = data[targetIdx];
                    
                    if (targetTile != '!' && targetTile != '#') {
                        cameFrom[targetIdx] = static_cast<char>(r + '0');
                        discoveredCount++; 

                        if (targetTile == 'C') {
                            int row = static_cast<int>(roomOffset / dim);
                            int col = static_cast<int>(roomOffset % dim);
                            finalPos = {targetRoom, row, col};
                            found = true;
                            break;
                        }
                        s.push_back(static_cast<size_t>(targetIdx));
                    }
                }
            }
        }
        else {
            // const int dRow[] = {-1, 0, 1, 0};
            // const int dCol[] = {0, 1, 0, -1};
            // const char dirs[] = {'n', 'e', 's', 'w'};
            size_t roomOffset = currentIdx % floorSize;
            size_t roomIdx = currentIdx / floorSize;
            int row = static_cast<int>(roomOffset / dim);
            int col = static_cast<int>(roomOffset % dim);
            // size_t baseRoomIdx = roomIdx * floorSize;

            // for (int i = 0; i < 4; ++i) {
            //     int nextRow = row + dRow[i];
            //     int nextCol = col + dCol[i];

            //     if (nextRow >= 0 && nextRow < size && nextCol >= 0 && 
            //         nextCol < size) {
            //         size_t nextIdx = baseRoomIdx + 
            //                         (static_cast<size_t>(nextRow) * dim) + 
            //                         static_cast<size_t>(nextCol);

            //         if (cameFrom[nextIdx] == 0) {
            //             char nextTile = data[nextIdx];

            //             if (nextTile != '#' && nextTile != '!') {
            //                 cameFrom[nextIdx] = dirs[i];
            //                 discoveredCount++; 

            //                 if (nextTile == 'C') {
            //                     finalPos = {static_cast<int>(roomIdx), 
            //                         nextRow, nextCol};
            //                     found = true;
            //                     break; 
            //                 }
            //                 s.push_back(static_cast<size_t>(nextIdx));
            //             }
            //         }
            //     }
            // }
            if (row > 0) {
            size_t nextIdx = currentIdx + northOffset;
                if (cameFrom[nextIdx] == 0 && data[nextIdx] != '#' && 
                    data[nextIdx] != '!') {
                    cameFrom[nextIdx] = 'n';
                    discoveredCount++;
                    if (data[nextIdx] == 'C') {
                        int nextRow = row - 1;
                        finalPos = {static_cast<int>(roomIdx), nextRow, col};
                        found = true;
                        break;
                    }
                    s.push_back(nextIdx);
                }
            }
            if (col < static_cast<int>(dim) - 1) {
                size_t nextIdx = currentIdx + eastOffset;
                if (cameFrom[nextIdx] == 0 && data[nextIdx] != '#' && 
                    data[nextIdx] != '!') {
                    cameFrom[nextIdx] = 'e';
                    discoveredCount++;
                    if (data[nextIdx] == 'C') {
                        int nextCol = col + 1;
                        finalPos = {static_cast<int>(roomIdx), row, nextCol};
                        found = true;
                        break;
                    }
                    s.push_back(nextIdx);
                }
            }
            if (row < static_cast<int>(dim) - 1) {
                size_t nextIdx = currentIdx + southOffset;
                if (cameFrom[nextIdx] == 0 && data[nextIdx] != '#' && 
                    data[nextIdx] != '!') {
                    cameFrom[nextIdx] = 's';
                    discoveredCount++;
                    if (data[nextIdx] == 'C') {
                        int nextRow = row + 1;
                        finalPos = {static_cast<int>(roomIdx), nextRow, col};
                        found = true;
                        break;
                    }
                    s.push_back(nextIdx);
                }
            }
            if (col > 0) {
                size_t nextIdx = currentIdx + westOffset;
                if (cameFrom[nextIdx] == 0 && data[nextIdx] != '#' && 
                    data[nextIdx] != '!') {
                    cameFrom[nextIdx] = 'w';
                    discoveredCount++;
                    if (data[nextIdx] == 'C') {
                        int nextCol = col - 1;
                        finalPos = {static_cast<int>(roomIdx), row, nextCol};
                        found = true;
                        break;
                    }
                    s.push_back(nextIdx);
                }
            }
        }
    }
    if (found) {
        return backtrace(finalPos, cameFrom);
    } else {
        cout << "No solution, " << discoveredCount << " tiles discovered.\n"; 
        return {};
    }
}

// helpter function to backtrace the path taken
vector<PathStep> routing::backtrace(std::vector<int> curr, const vector<char> 
    &cameFrom) {
    vector<PathStep> path;
    size_t dim = static_cast<size_t>(size);
    size_t floorSize = dim * dim;
    path.reserve(discoveredCount);

    while (true) {
        size_t currIdx = (static_cast<size_t>(curr[0]) * floorSize) + 
                         (static_cast<size_t>(curr[1]) * dim) + 
                         static_cast<size_t>(curr[2]);
        
        char moveUsed = cameFrom[currIdx];
        vector<int> prev = curr; 
        if (moveUsed == 'S') {
            break; 
        }

        if (moveUsed == 'n') prev[1]++;
        else if (moveUsed == 's') prev[1]--; 
        else if (moveUsed == 'e') prev[2]--; 
        else if (moveUsed == 'w') prev[2]++; 
        else if (moveUsed >= '0' && moveUsed <= '9') {
            prev[0] = moveUsed - '0';
            moveUsed = 'p';
        }
        path.push_back({prev[0], prev[1], prev[2], moveUsed});
        curr = prev;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

// Print Functions
void routing::printMapOutput(const vector<PathStep>& path) {
    const size_t dim = static_cast<size_t>(size);
    const size_t floorSize = dim * dim;
    for (const auto& step : path) {
    size_t idx = (static_cast<size_t>(step.room) * floorSize) + 
                 (static_cast<size_t>(step.row) * dim) + 
                 static_cast<size_t>(step.col);
        data[idx] = step.dir;
    }

    cout << "Start in room " << path[0].room << ", row " << path[0].row 
         << ", column " << path[0].col << "\n";

    for (int i = 0; i < rooms; ++i) {
        cout << "//castle room " << i << "\n";
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                size_t idx = (static_cast<size_t>(i) * floorSize) + 
                         (static_cast<size_t>(r) * dim) + 
                         static_cast<size_t>(c);
                cout << data[idx];
            }
            cout << "\n";
        }
    }
}

void routing::printListOutput(const vector<PathStep>& path) {
    cout << "Path taken:\n";  
    for (const auto& step : path) {
        cout << "(" << step.room << "," << step.row << "," << step.col 
             << "," << step.dir << ")\n";
    }
}