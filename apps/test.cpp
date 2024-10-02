#include <iostream>
#include <vector>

#include "specific/tsp.hpp"
#include "generic/optimizer.hpp"

using namespace std;

int main(int, char**){
    vector<vector<uint>> dist = {
        {0, 12, 29, 23, 13, 24}, 
        {12, 0, 19, 3, 25, 6},
        {29, 19, 0, 21, 23, 28},
        {22, 3, 21, 0, 4, 5},
        {13, 25, 23, 4, 0, 16},
        {24, 6, 28, 5, 16, 0}
    };

    Matrix distance_matrix(dist);

    // TSP_instance inst("tsp", distance_matrix);

    TSP_instance inst("tsp", "../data/tsplib/a280.json");   

    inst.print(cout);
    cout << endl;

    Optimizer opt(&inst);

    opt.get_curr_sol().print(cout);
    cout << endl;

    while (true) {
        if (!opt.swap())
            break;
        opt.get_curr_sol().print(cout);
        cout << endl;
    }
    

    // opt.get_curr_sol().print(cout);
    // cout << endl;
}