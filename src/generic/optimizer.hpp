#pragma once

#include <vector>

#include "generic/instance.hpp"
#include "generic/solution.hpp"

class Optimizer
{
public:
	Optimizer(Instance& instance);

	Solution get_curr_sol() { return this->curr_sol; }

private:
	Instance instance;

	Solution init_sol;
	Solution curr_sol;

	static Solution get_naive_solution(Instance& instance);

	

	// local search
	bool insert();
};