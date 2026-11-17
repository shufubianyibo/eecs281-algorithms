// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA262043

#include "battle.hpp"
using namespace std;

void battle::add_median_data(uint32_t lifespan) {
    if (low.empty() || lifespan <= low.top()) {
        low.push(lifespan);
    } else {
        high.push(lifespan);
    }
    if (low.size() > high.size() + 1) {
        high.push(low.top());
        low.pop();
    } else if (high.size() > low.size() + 1) {
        low.push(high.top());
        high.pop();
    }
}
uint32_t battle::get_median() {
    if (low.size() == high.size()) {
        return (low.top() + high.top()) / 2;
    } else if (low.size() > high.size()) {
        return low.top();
    } else {
        return high.top();
    }
}
void battle::printHelp(char *command) {
    cout << "Usage: " << command << "--verbose --statistics <num> --median\n";
    cout << "  --verbose               Optional, print the operations of the program"
    " about zombies being created, moved, or destroyed\n";
    cout << "  --statistics <num>      Optional, print <num> entries for each type "
    "of statistic\n";
    cout << "  --median                Optional, print the median time that zombies "
    "have been active before being destroyed\n";
    cout << "  --help                  Show this help message\n" << flush;
} 
    
void battle::getOptions(int argc, char **argv) {
    opterr = static_cast<int>(false);  
    int choice = 0;
    int index = 0;

    option longOptions[] = {
        {"verbose", no_argument, nullptr, 'v'},
        {"statistics", required_argument, nullptr, 's'},
        {"median", no_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'},
    };  
    
    while ((choice = getopt_long(argc, argv, "vs:mh", 
        static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'v': {  
            verbose = true;
            break;
        } 

        case 's': {
            stats_mode = true;
            num_stats = static_cast<uint32_t>(stoi(optarg));
            break;
        }

        case 'm': {
            median_mode = true;
            break;
        }

        default:
            cerr << "Unknown command line option\n" << flush;
            exit(1);

        }
    } 
} 

void battle::readHeader(){
    string junk;
    getline(cin, junk);
    cin >> junk >> quiver_capacity;
    cin >> junk >> random_seed;
    cin >> junk >> max_random_distance;
    cin >> junk >> max_random_speed;
    cin >> junk >> max_random_health;
    P2random::initialize(random_seed, max_random_distance, 
        max_random_speed, max_random_health);
}
    

void battle::play(){
    string junk;
    uint32_t file_round;
    if (!(cin >> junk >> junk >> file_round)) more_round_exist = false;
    
    string killer;
    string last_killed_name; 

    vector<zombie*> active_refs; 
    active_refs.reserve(5000);
    vector<zombie*> active_move;
    active_move.reserve(5000);
    
    while (player_alive && (!active_refs.empty() || more_round_exist)){  
        round_num++;
        if (verbose) cout << "Round: " << round_num << "\n";
        arrows = quiver_capacity;
        
        // move 
        for (zombie* z : active_move){
            z->move();
            if (verbose) cout << "Moved: " << z->name << " (distance: " 
            << z->distance << ", speed: " << z->speed << ", health: " 
            << z->health << ")\n";
            if (z->distance == 0) {
                if (player_alive) {
                    killer = z->name;
                    player_alive = false;
                }
            } 
        }
        if (!player_alive) {
            cout << "DEFEAT IN ROUND " << round_num << "! " << killer << 
            " ate your brains!\n";
            for (zombie* z : active_move) {
                if (z->active) z->update_lifespan(round_num);
            }
            break;
        }

        // new zombies
        if (file_round == round_num && more_round_exist) {
            uint32_t random_cnt, named_cnt;
            cin >> junk >> random_cnt >> junk >> named_cnt;

            for (uint32_t i = 0; i < random_cnt; i++) {
                zombie* new_z = new zombie(P2random::getNextzombieName(), P2random::getNextzombieDistance(), P2random::getNextzombieSpeed(), P2random::getNextzombieHealth(), round_num);
                zombies.push_back(new_z);  
                active_refs.push_back(new_z);
                push_heap(active_refs.begin(), active_refs.end(), CompareETA());
                active_move.push_back(new_z);
                if (verbose) cout << "Created: " << new_z->name << " (distance: " 
                << new_z->distance << ", speed: " << new_z->speed << ", health: " 
                << new_z->health << ")\n";
            }
            for (uint32_t i = 0; i < named_cnt; i++){
                string name; uint32_t d, s, h;
                cin >> name >> junk >> d >> junk >> s >> junk >> h;
                zombie* new_z = new zombie(name, d, s, h, round_num);
                zombies.push_back(new_z); 
                active_refs.push_back(new_z);
                push_heap(active_refs.begin(), active_refs.end(), CompareETA());
                active_move.push_back(new_z);
                if (verbose) cout << "Created: " << name << " (distance: " 
                << d << ", speed: " << s << ", health: " << h << ")\n";
            }
            if (!(cin >> junk >> junk >> file_round)) more_round_exist = false;
        }

        // player defense
        while (arrows > 0 && !active_refs.empty()) {
            zombie* target = active_refs.front();
            if (!target->active) {
                pop_heap(active_refs.begin(), active_refs.end(), CompareETA());
                active_refs.pop_back();
                continue;
            }
            std::pop_heap(active_refs.begin(), active_refs.end(), CompareETA());
            
            uint32_t damage = min(arrows, target->health);
            target->take_damage(damage);
            arrows -= damage;
            
            if (target->health == 0) {
                target->death_round = round_num;
                target->update_lifespan(round_num);
                last_killed_name = target->name; 

                if (stats_mode) killed_zombies.push_back(target);
                if (median_mode) add_median_data(target->rounds_active);
                if (verbose) cout << "Destroyed: " << target->name << " (distance: " 
                << target->distance << ", speed: " << target->speed << ", health: " 
                << target->health << ")\n";
                
                target->active = false;
                active_refs.pop_back(); 
            } else {
                std::push_heap(active_refs.begin(), active_refs.end(), CompareETA());
            }
        }

        size_t write = 0;
        for (size_t read = 0; read < active_move.size(); ++read) {
            if (active_move[read]->active) {
                active_move[write++] = active_move[read];
            }
        }
        active_move.resize(write);
        
        if (median_mode && !low.empty()) cout << "At the end of round " 
        << round_num << ", the median zombie lifetime is " << get_median() << "\n";

    } 

    if (player_alive) {
        if (!last_killed_name.empty()) {
            cout << "VICTORY IN ROUND " << round_num << "! " << 
            last_killed_name << " was the last zombie.\n";
        } 
    }
}

void battle::print_statistics() {
    if (!stats_mode) return;
    size_t active_count = 0;
    for (const auto* z : zombies) { 
        if (z->active) active_count++;
    }
    cout << "Zombies still active: " << active_count << "\n";

    cout << "First zombies killed:\n";
    size_t print_count = min((size_t)num_stats, killed_zombies.size());
    for (size_t i = 0; i < print_count; ++i) {
        cout << killed_zombies[i]->name << " " << (i + 1) << "\n";
    }

    cout << "Last zombies killed:\n";
    size_t start_index = killed_zombies.size() > 
    num_stats ? killed_zombies.size() - num_stats : 0;
    for (size_t i = killed_zombies.size(); i > start_index; --i) {
        cout << killed_zombies[i-1]->name << " " << (i-start_index) << "\n";
    }
    
    std::sort(zombies.begin(), zombies.end(), 
    [](const zombie* a, const zombie* b) {
        if (a->rounds_active != b->rounds_active) {
            return a->rounds_active > b->rounds_active;
        }
        return a->name < b->name; 
    });
    cout << "Most active zombies:\n";
    size_t count = min((size_t)num_stats, zombies.size());
    for (size_t i = 0; i < count; ++i) {
        cout << zombies[i]->name << " " << 
        zombies[i]->rounds_active << "\n";}

    // Least Active
    std::sort(zombies.begin(), zombies.end(), 
    [](const zombie* a, const zombie* b) {
        if (a->rounds_active != b->rounds_active) {
            return a->rounds_active < b->rounds_active;}
        return a->name < b->name; 
    });
    cout << "Least active zombies:\n";
    for (size_t i = 0; i < count; ++i) {
        cout << zombies[i]->name << " " << 
        zombies[i]->rounds_active << "\n";
}}
battle::~battle() {
    for (zombie* z : zombies) {
        delete z;
    }
}