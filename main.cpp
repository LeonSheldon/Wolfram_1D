#include <iostream>
#include "stdint.h"
#include <tuple>
#include <vector>
#include <list>
#include <string>

typedef std::vector<bool> world_t;
typedef std::tuple<bool, bool, bool> partition_t;

#define DEFAULT_CELL false

/*
 * A program that calculates and prints Stephen Wolfram's 1D Cellular automata
 * Leon Sheldon
 * June 2017
 */

// Prints a world
void print_world(world_t &world) {
    for (bool cell : world) {
        std::cout << (cell ? "." : " ");
    }
    std::cout << std::endl;
}

// Given a partition, generate a new cell underneath it
bool generate_new_cell_by_rule(partition_t &partition, bool* &transition_function) {
    uint8_t ix = (std::get<0>(partition) ? 0 : 4) + (std::get<1>(partition) ? 0 : 2) + (std::get<2>(partition) ? 0 : 1);
    return transition_function[ix];
}

// Groups each section of three cells
std::list<partition_t> generate_partitions(world_t &world) {
    std::list<partition_t> partitions;
    bool back_two_cells = DEFAULT_CELL;
    bool back_one_cell  = DEFAULT_CELL;
    for (bool current_cell : world) {
        partitions.push_back(std::make_tuple(back_two_cells, back_one_cell, current_cell));
        back_two_cells = back_one_cell;
        back_one_cell  = current_cell;
    }
    // we made one too many at the beginning, and missed one at the end
    partitions.push_back(std::make_tuple(back_two_cells, back_one_cell, DEFAULT_CELL));
    partitions.pop_front();
    return partitions;
}

// Transitions a given world to the next iteration
world_t generate_new_world(world_t &old_world, bool* transition_function) {
    world_t new_world;
    std::list<partition_t> partitions = generate_partitions(old_world);
    for (partition_t partition : partitions) {
        new_world.push_back(generate_new_cell_by_rule(partition, transition_function));
    }
    return new_world;
}

// An empty world with a single cell on in the middle
world_t initial_world(int &length) {
    int starting_cell_location = length / 2;
    world_t world;
    for (int i = 0; i < length; i++) {
        world.push_back(i == starting_cell_location);
    }
    return world;
}
// Converts between the rule numbers used by Wolfram, and the array of bools
// that we actually use to encode the rule
bool* rule_no_to_array(uint8_t rule) {
    uint8_t temp = rule;
    static bool ret[8];
    for (int i = 7; i >= 0; i--) {
        ret[i] = temp % 2 != 0;
        temp /= 2;
    }
    return ret;
}

int main()
{
    int world_size = 80;
    bool* transition_rule = rule_no_to_array(90);
    world_t world = initial_world(world_size);
    for (int i = 0; i < world_size; i++) {
        print_world(world);
        world = generate_new_world(world, transition_rule);
    }
    return 0;
}
