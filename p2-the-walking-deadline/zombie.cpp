// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA262043
#include <algorithm>
#include <iostream>
#include <vector> 
#include "zombie.hpp"
using namespace std;

zombie::zombie(string name, uint32_t dist, uint32_t speed, 
    uint32_t health, uint32_t round) : name(name), distance(dist), 
    speed(speed), health(health), born_round(round) {
    if (speed == 0) {
        eta = UINT32_MAX; 
        eta_priority = UINT32_MAX;
    } else {
        eta = distance / speed;
        eta_priority = (dist / speed) + born_round;
    }
}
zombie::~zombie() {}        
void zombie::take_damage(uint32_t damage) {
    if (damage >= health) {
        health = 0;
        active = false;
    } else {
        health -= damage;
    }
}
void zombie::move() {
    distance -= min(distance, speed);
    if (eta != UINT32_MAX && eta > 0) {
        --eta;
    }
}

uint32_t zombie::getETA() {
    return eta;
}

void zombie::update_lifespan(uint32_t current_round) {
    rounds_active = current_round - born_round + 1;
}