#pragma once

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "config.hpp"

class Solution
{
public:
    std::vector<uint> permutation;
    std::vector<uint> frequency;
    bool feasible;
    fitness_t fitness;

    Solution(uint node_count=0);
    // Solution(Solution& sol);
    Solution(std::vector<uint>& perm, std::vector<uint> &freq, 
        bool& feasible, fitness_t& fitness);



    bool empty() { return this->size() == 0; }
    uint size() { return this->permutation.size(); }

    static bool verify_perm_freq(std::vector<uint> perm, std::vector<uint> freq);

    void print(std::ostream& os);

    friend std::ostream& operator << (std::ostream& os, Solution& sol);
};