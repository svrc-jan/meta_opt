#include "optimizer.hpp"


Optimizer::Optimizer(Instance* instance) 
{
	this->instance = instance;

	// this->init_sol = this->get_naive_solution();
	this->init_sol = this->get_greedy_solution();
	this->curr_sol = this->init_sol;
}

Solution Optimizer::get_naive_solution()
{
	Solution sol;
	
	uint sol_size = std::accumulate(this->instance->lbs.begin(), this->instance->lbs.end(), 0);
	sol.permutation.reserve(sol_size);

	sol.frequency = this->instance->lbs;

	for (uint i = 0; i < sol.frequency.size(); i++) {
		for (uint j = 0; j < sol.frequency[i]; j++) {
			sol.permutation.push_back(i);
		}
	}

	sol.feasible = this->instance->get_fitness(sol.fitness, sol.permutation, sol.frequency);

	return sol;
}

Solution Optimizer::get_greedy_solution()
{
	this->curr_sol = Solution(this->instance->node_cnt);
	while (!this->instance->is_lb_met(this->curr_sol.frequency)) {
		this->insert();
		// std::cout << this->curr_sol << std::endl;
	}
	
	return this->curr_sol;
}

bool Optimizer::insert()
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = curr_sol;

	const uint perm_size = perm.size();

	bool updated = false;

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint node = 0; node < this->instance->node_cnt; node++) {
		if (this->instance->use_ubs)
			if (freq[node] >= this->instance->ubs[node] && this->instance->ubs[node] > 0)
				continue;

		std::vector<uint> new_freq = freq;
		new_freq[node]++;

		fitness_t lb_pen = this->instance->get_lb_penalty(new_freq);

		std::vector<uint> new_perm(perm_size + 1);
		// memcpy(&(new_perm[1]), &(perm[0]), sizeof(uint)*perm_size);
		std::copy(&perm[0], &perm[perm_size], &new_perm[1]);

		for (uint pos = 0; pos < perm_size + 1; pos++) {

			new_perm[pos] = node;

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, new_freq) && (lb_pen == 0) ;
			new_fitness += lb_pen;

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, new_freq, new_feasible, new_fitness);
					updated = true;
				}
			}	

			if (pos < perm_size)
				new_perm[pos] = perm[pos];
		}
	}

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}

bool Optimizer::remove()
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = this->curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

	// fitness_t lb_pen = this->instance->get_lb_penalty(new_freq);

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i < perm_size; i++) {
		if (freq[perm[i]] <= this->instance->lbs[perm[i]]) {
			continue;
		}
		std::vector<uint> new_freq = freq;
		std::vector<uint> new_perm(perm_size - 1);

		new_freq[perm[i]]--;
		if (i > 0)
			// memcpy(&(new_perm[0]), &(perm[0]), sizeof(uint)*i);
			std::copy(&perm[0], &perm[i], &new_perm[0]);
		if (i < perm_size - 1)
			// memcpy(&(new_perm[i]), &(perm[i+1]), sizeof(uint)*(perm_size - i - 1));
			std::copy(&perm[i+1], &perm[perm_size], &new_perm[i]);

		fitness_t new_fitness = 0;
		bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, new_freq);

#pragma omp critical
		{
			if (new_fitness < best_sol.fitness) {
				best_sol = Solution(new_perm, new_freq, new_feasible, new_fitness);
				updated = true;
			}
		}
	}

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}

bool Optimizer::move(uint p, bool reverse)
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = this->curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i < perm_size; i++) {
		std::vector<uint> new_perm = perm;

		for (uint j = i + 2; j < perm_size; j++) {
			std::reverse_copy(&perm[i], &perm[j], &new_perm[i]);

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
					updated = true;
				}
			}
		}
	}

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}

bool Optimizer::two_opt()
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = this->curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i < perm_size; i++) {
		std::vector<uint> new_perm = perm;

		for (uint j = i + 2; j < perm_size; j++) {
			std::reverse_copy(&perm[i], &perm[j], &new_perm[i]);

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
					updated = true;
				}
			}
		}
	}

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}

bool Optimizer::exchange(uint p, uint q)
{
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

	// fitness_t lb_pen = this->instance->get_lb_penalty(new_freq);

	for (uint i = 0; i < perm_size; i++) {
		std::vector<uint> new_perm = perm;
		
		for (uint j = i + 1; i < perm_size; i++) {
			new_perm[j] = perm[i];
			new_perm[i] = perm[j];

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

			if (new_fitness < best_sol.fitness) {
				best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
				updated = true;
			}

			// repair sol
			new_perm[j] = perm[j];
			new_perm[i] = perm[i];
		}
	}

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}

