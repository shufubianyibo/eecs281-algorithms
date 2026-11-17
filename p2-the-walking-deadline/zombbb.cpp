// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA262043
#include <algorithm>
#include <iostream>
#include <vector> 
#include <queue>  
#include "battle.hpp"
#include "zombie.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    battle b;
    b.getOptions(argc, argv);
    b.readHeader();
    b.play();
    b.print_statistics();
    return 0;
}