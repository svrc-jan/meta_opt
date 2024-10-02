#pragma once

#include <vector>
#include <cstring>

#include "generic/instance.hpp"
#include "generic/solution.hpp"

class Optimizer
{
public:
	Optimizer(Instance* instance);

	Solution get_curr_sol() { return this->curr_sol; }

private:
	Instance* instance;

	Solution init_sol;
	Solution curr_sol;

	Solution get_naive_solution();
	Solution get_greedy_solution();
	
public:
	// local search
	bool insert();
	bool remove();
	bool two_opt();
	bool move(uint p, bool reverse);
	bool exchange(uint p, uint q);

};