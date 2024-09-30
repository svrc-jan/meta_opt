#include "optimizer.hpp"


Optimizer::Optimizer(Instance& instance) 
{
	this->instance = instance;

	this->init_sol = this->get_naive_solution(instance);
	this->curr_sol = this->init_sol;
}

Solution Optimizer::get_naive_solution(Instance& instance)
{
	Solution sol;
	
	uint sol_size = std::accumulate(instance.lbs.begin(), instance.lbs.end(), 0);
	sol.permutation.reserve(sol_size);

	sol.frequency = instance.lbs;

	for (uint i = 0; i < sol.frequency.size(); i++) {
		for (uint j = 0; j < sol.frequency[i]; j++) {
			sol.permutation.push_back(i);
		}
	}

	sol.feasible = instance.get_fitness(sol.fitness, sol.permutation, sol.frequency);

	return sol;
}

bool Optimizer::insert()
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	bool updated = false;


	for (uint n = 0; n < this->instance.node_cnt; n++) {
		std::vector<uint> new_freq = freq;
		new_freq[n]++;

		std::vector<uint> new_perm(perm.size() + 1);
		

	}
}