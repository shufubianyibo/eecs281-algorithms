// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA262043
#pragma once
#include <iostream>
#include <vector> 
#include <queue>
#include <algorithm>
#include "zombie.hpp"
#include <getopt.h>
#include "P2random.h"
using namespace std;

class battle {
    private:
        vector<zombie*> zombies;
        vector<zombie*> killed_zombies;
        struct CompareETA {
            bool operator()(const zombie* a, const zombie* b) const {
                if (a->eta_priority == b->eta_priority) {
                    if (a->health == b->health) {
                        return a->name > b->name;
                    }
                    return a->health > b->health;
                }
                return a->eta_priority > b->eta_priority;
            }
        };
        priority_queue<zombie*, vector<zombie*>, CompareETA> pq;
        uint32_t round_num = 0;
        uint32_t quiver_capacity = 0;
        uint32_t arrows = 0;
        uint32_t random_seed = 0;
        bool player_alive = true;
        bool verbose = false;
        bool median_mode = false;
        bool stats_mode = false;
        uint32_t num_stats = 0;
        uint32_t max_random_distance = 0;
        uint32_t max_random_speed = 0;
        uint32_t max_random_health = 0;
        bool more_round_exist = true;
        priority_queue<uint32_t> low; 
        priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> high;
        void add_median_data(uint32_t lifespan);
        uint32_t get_median();
        void printHelp(char *command);

    public: 
        void getOptions(int argc, char **argv);
        void readHeader();
        void play();
        void print_statistics();
        ~battle();
};