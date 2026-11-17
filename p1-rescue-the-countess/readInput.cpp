// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "readInput.hpp"
#include "routing.hpp"   
using namespace std;


// constructor
readInput::readInput() : rooms(0), size(0) {};


int readInput::getRooms(){
    return rooms;
}

int readInput::getSize(){
    return size;
}

const vector<int>& readInput::getStartPosition(){
    return startPosition;
}

vector<char>& readInput::getData(){
    return data;
}


void readInput::readData() {
        std::ios::sync_with_stdio(false);
        cin.tie(NULL);
        string mode;
        if (!(cin >> mode >> rooms >> size)) {
        cerr << "Unknown command line option\n";
        exit(1);}

        if (rooms <= 0 || rooms > 10) {
            cerr << "Invalid room number\n";
            exit(1);
        }
        if (size <= 0) {
            cerr << "Invalid row number\n";
            exit(1);
        }
        size_t numRooms = static_cast<size_t>(rooms);
        size_t dim = static_cast<size_t>(size);

        data.assign(numRooms * dim * dim, '.');
        
        if (mode == "M") {
            readMapMode();
        } 
        else if (mode == "L") {
            readListMode();
        } 
        else {
            cerr << "Unknown command line option\n";
            exit(1);
        }
        if (startPosition[0] == -1 || startPosition[1] == -1 || 
            startPosition[2] == -1) {
            cerr << "Error: Start position 'S' not found in input\n";
            exit(1);
        }
    }




// read the map mode input file and store it in a 3D array
void readInput::readMapMode() {
        string line;
        size_t dim = static_cast<size_t>(size);
        size_t floorArea = dim * dim;
        for (int r = 0; r < rooms; ++r) {
            int i = 0;
            while (i < size) {
                cin >> line;
                // Skip comments
                if (line.length() >= 2 && line[0] == '/' && 
                line[1] == '/') {
                    string dummy;
                    getline(cin, dummy);
                    continue;
                }
                size_t len = line.length();
                if (len > dim) len = dim;

                for (size_t j = 0; j < len; ++j) {
                char c = line[j];
                bool isValid = (c == '.' || c == '#' || c == '!' || 
                    c == 'C' || c == 'S' || (c >= '0' && c <= '9'));
                if (!isValid) {
                    cerr << "Unknown map character\n";
                    exit(1);
                }
                size_t idx = (static_cast<size_t>(r) * floorArea) + 
                             (static_cast<size_t>(i) * dim) + j;
            
                data[idx] = c;
                if (c == 'S') {
                    startPosition = {r, i, static_cast<int>(j)}; 
                }
                }
                ++i;
            }
        }
    }

// read the list mode input file and store it in a 2D array
void readInput::readListMode() {
        char startChar;
        size_t dim = static_cast<size_t>(size);
        size_t floorArea = dim * dim;
        while (cin >> startChar) {
            if (startChar == '/') {
                string dummy;
                getline(cin, dummy); 
                continue;
            }

            if (startChar == '(') {
                int roomIdx, row, col;
                char val, comma, closeParen;
                
                cin >> roomIdx >> comma >> row >> comma >> col >> comma;
                cin >> val >> closeParen;
                bool isValid = (val == '.' || val == '#' || val == '!' || 
                    val == 'C' || val == 'S' || (val >= '0' && val <= '9'));
                if (!isValid) {
                    cerr << "Unknown map character\n";
                    exit(1);
                }

                if (roomIdx >= 0 && roomIdx < rooms && row >= 0 && 
                    row < size && col >= 0 && col < size) {
                    size_t idx = (static_cast<size_t>(roomIdx) * floorArea) + 
                             (static_cast<size_t>(row) * dim) + 
                             static_cast<size_t>(col);
                    data[idx] = val;
                    if (val == 'S') {
                        startPosition = {roomIdx, row, col};
                    }
                } else if (roomIdx >= rooms) {
                    cerr << "Invalid room number\n";
                    exit(1);
                } else if (row >= size) {
                    cerr << "Invalid row number\n";
                    exit(1);
                } else if (col >= size) {
                    cerr << "Invalid column number\n";
                    exit(1);
                }
            }
        }
    }


