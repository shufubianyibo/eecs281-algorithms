// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA262043
#pragma once
#include <algorithm>
#include <iostream>
#include <vector> 
using namespace std;

class zombie {
    private:
        
        
    public:
        string name;
        uint32_t distance;
        uint32_t speed;
        uint32_t health;
        uint32_t born_round;
        uint32_t death_round;
        uint32_t eta_priority;
        bool active = true;
        uint32_t eta;
        uint32_t rounds_active = 0;
        //constructor
        zombie(std::string name, uint32_t dist, 
            uint32_t speed, uint32_t health, uint32_t round);
        //destructor
        ~zombie();
        void take_damage(uint32_t damage);
        void move();
        uint32_t getETA();
        void update_lifespan(uint32_t current_round);

};