// Project Identifier: B99292359FFD910ED13A7E6C7F9705B874262D79
#pragma once
#include <vector>
using namespace std; 

class readInput {
private:
int rooms;
int size;
vector<int> startPosition = {-1, -1, -1};
vector<char> data;

public:
// constructor
readInput();
int getRooms();
int getSize();
const vector<int>& getStartPosition();
vector<char>& getData();
void readData();

private:
void readMapMode();
void readListMode();
};