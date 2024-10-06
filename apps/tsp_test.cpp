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
    string file_name = "../data/tsplib/d198.json";

    if (argc > 1) {
        file_name = string(argv[1]);
    }

    TSP_instance inst("tsp", file_name);   

    inst.print(cout);
    cout << endl;

    Optimizer opt(&inst);

    opt.get_best_sol().print(cout);
    cout << endl;

    opt.set_operation_list({
        "two_opt",
        "move_1",
        "move_2",
        "move_3",
        "move_4",
        "move_5",
        "move_rev_2",
        "move_rev_3",
        "exchange_1_1",
        "exchange_2_1",
        "exchange_1_2",
        "exchange_2_2",
        "exchange_2_3",
        "exchange_3_2",
        "exchange_3_3",
        "exchange_2_4",
        "exchange_4_2",
        "exchange_3_4",
        "exchange_4_3",
        "exchange_4_4",
        "exchange_rev_2_1",
        "exchange_rev_1_2",
        "exchange_rev_2_2",
        "exchange_rev_2_3",
        "exchange_rev_3_2",
        "exchange_rev_3_3"
    });
    // opt.set_all_operation_list();
    opt.set_perturbation("double_bridge");
    // opt.set_local_search("cyclic");
    opt.set_local_search("cyclic");
    opt.set_metaheuristic("ils");

    opt.run();

    cout << "Final ";
    opt.get_best_sol().print(cout);
    cout << endl;
}