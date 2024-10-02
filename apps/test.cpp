#include <iostream>
#include <vector>
#include <omp.h>

#include "specific/tsp.hpp"
#include "generic/optimizer.hpp"

using namespace std;

int main(int argc, char** argv){
    vector<vector<uint>> dist_mtx = {
        {0, 12, 29, 23, 13, 24}, 
        {12, 0, 19, 3, 25, 6},
        {29, 19, 0, 21, 23, 28},
        {22, 3, 21, 0, 4, 5},
        {13, 25, 23, 4, 0, 16},
        {24, 6, 28, 5, 16, 0}
    };

    // TSP_instance inst("tsp", dist_mtx);
    // omp_set_num_threads(8);
    string file_name = "../data/tsplib/a280.json";

    if (argc > 1) {
        file_name = string(argv[1]);
    }

    TSP_instance inst("tsp", file_name);   

    inst.print(cout);
    cout << endl;

    Optimizer opt(&inst);

    opt.get_curr_sol().print(cout);
    cout << endl;

    while (true) {
        if (!opt.two_opt())
            break;
        // opt.get_curr_sol().print(cout);
        // cout << endl;
    }


    
    // cout << "omp thread count " << omp_get_num_threads() << "/" << omp_get_max_threads() << endl;
    // opt.get_curr_sol().print(cout);
    cout << endl;
}