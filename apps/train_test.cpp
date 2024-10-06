
#include <iostream>
#include <vector>
#include <omp.h>

#include "specific/train_disp.hpp"
#include "generic/optimizer.hpp"

using namespace std;

int main(int argc, char** argv)
{
	string file_name = "../data/train_disp/displib_testinstances_headway1.json";

    if (argc > 1) {
        file_name = string(argv[1]);
    }

    Train_disp_inst inst("train_disp", file_name);
}