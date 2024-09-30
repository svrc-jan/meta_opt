#include <iostream>
#include <vector>

#include "specific/tsp.hpp"
#include "generic/optimizer.hpp"

using namespace std;

int main(int, char**){
    vector<vector<uint>> dist_mtx = {
        {0, 12, 29, 23, 13, 24}, 
        {12, 0, 19, 3, 25, 6},
        {29, 19, 0, 21, 23, 28},
        {22, 3, 21, 0, 4, 5},
        {13, 25, 23, 4, 0, 16},
        {24, 6, 28, 5, 16, 0}
    };

    TSP_instance inst("tsp", dist_mtx);

    inst.print(cout);
    cout << endl;

    Optimizer opt(inst);

    opt.get_curr_sol().print(cout);
    cout << endl;
}