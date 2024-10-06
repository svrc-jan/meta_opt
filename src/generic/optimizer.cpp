#include "optimizer.hpp"


Optimizer::Optimizer(Instance* instance, uint seed) 
{
	this->instance = instance;

	// this->init_sol = this->get_naive_solution();
	this->init_sol = this->get_greedy_solution();
	this->curr_sol = this->init_sol;
	this->best_known_sol = this->init_sol;

	this->init_rng(seed);
}

Optimizer::~Optimizer()
{
	delete this->rng;
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
	}
	
	return this->curr_sol;
}

void Optimizer::set_operation_list(std::vector<std::string> op_names)
{

	this->operation_list.clear();
	this->operation_names.clear();
	for (std::string op_name : op_names) {
		try {
			auto op = this->operation_map.at(op_name);
			this->operation_list.push_back(op);
			this->operation_names.push_back(op_name);
		}
		catch (const std::out_of_range&) {
			std::cerr << "ERROR: Invalid operator name: '" << op_name << "'" << std::endl;
		}
	}

#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Operation list set to ";
	print_vec(std::cout, this->operation_names);
	std::cout << std::endl;
#endif

}

void Optimizer::set_all_operation_list()
{
	this->operation_names.clear();
	this->operation_list.clear();
	for (const auto &op : this->operation_map) {
		this->operation_names.push_back(op.first);
		this->operation_list.push_back(op.second);
	}
}

void Optimizer::set_perturbation(std::string perturbation_name)
{
	try {
		this->perturbation = this->perturbation_map.at(perturbation_name);
		this->perturbation_name = perturbation_name;

#if defined STDOUT_ENABLED && STDOUT_ENABLED
		std::cout << "Perturbation set to '" << perturbation_name << "'" << std::endl;
#endif

	}
	catch (const std::out_of_range&) {
		std::cerr << "ERROR: Invalid perturbation name: '" << perturbation_name << "'" << std::endl;
	}
}

void Optimizer::set_local_search(std::string local_search_name)
{
	try {
		this->local_search = this->local_search_map.at(local_search_name);
		this->local_search_name = local_search_name;

#if defined STDOUT_ENABLED && STDOUT_ENABLED
		std::cout << "Local search set to '" << local_search_name << "'" << std::endl;
#endif

	}
	catch (const std::out_of_range&) {
		std::cerr << "ERROR: Invalid local search name: '" << local_search_name << "'" << std::endl;
	}
}

void Optimizer::set_metaheuristic(std::string metaheuristic_name)
{
	try {
		this->metaheuristic = this->metaheuristic_map.at(metaheuristic_name);
		this->metaheuristic_name = metaheuristic_name;

#if defined STDOUT_ENABLED && STDOUT_ENABLED
		std::cout << "Metaheuristic set to '" << metaheuristic_name << "'" << std::endl;
#endif

	}
	catch (const std::out_of_range&) {
		std::cerr << "ERROR: Invalid metaheuristic name: '" << metaheuristic_name << "'" << std::endl;
	}
}

// METAHEURISTICS
void Optimizer::ils()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Metaheurisric: ILS" << std::endl;
#endif
	uint k = 5;
	
	uint no_impr_iter = 0;
	uint no_impr_max = 20;
	
	while (true) {
		std::cout << "ILS Iter: " << no_impr_iter + 1 << "/" << no_impr_max << std::endl;
		this->local_search();

		if (this->curr_sol.fitness < this->best_known_sol.fitness) {
			this->best_known_sol = this->curr_sol;

#if defined STDOUT_ENABLED && STDOUT_ENABLED
			std::cout << "New best " << this->best_known_sol << std::endl;
#endif
			no_impr_iter = 0;
		}
		else {
			no_impr_iter++;
		}

		if (no_impr_iter >= no_impr_max)
			break;

		this->curr_sol = this->best_known_sol;
		this->perturbation(k);
	}
}

// PERMUTATIONS
void Optimizer::double_bridge(uint k, bool reverse_all)
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Perturbation: Double bridge" << (reverse_all ? " reverse all " : " ") << k << std::endl;
#endif

	std::vector<uint>& perm = this->curr_sol.permutation;

	std::uniform_int_distribution<uint> k_dist(0, perm.size());
	std::uniform_int_distribution<uint> rev_dist(0, 2);

	std::vector<uint> indexes(k+1);
	for (uint i = 0; i < k; i++) {
		indexes[i] = k_dist(*this->rng);
	}
	indexes[k] = perm.size();
	std::sort(indexes.begin(), indexes.end());


	std::vector<uint> new_perm(perm.size());

	bool reverse;
	uint idx, prev_idx = 0;
	for (uint i = 0; i <= k; i++) {
		if (reverse_all)
			reverse = true;
		else
			reverse = (bool)rev_dist(*this->rng);

		idx = indexes[i];
		if (reverse)
			std::reverse_copy(&perm[prev_idx], &perm[idx], &new_perm[prev_idx]);
		
		else
			std::copy(&perm[prev_idx], &perm[idx], &new_perm[prev_idx]);

		prev_idx = idx;
	}

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
	assert(Solution::verify_perm_freq(new_perm, this->curr_sol.frequency));
#endif

	this->curr_sol.permutation = new_perm;
	this->curr_sol.feasible = this->instance->get_fitness(
		this->curr_sol.fitness,
		this->curr_sol.permutation,
		this->curr_sol.frequency);

}

// LOCAL SEARCH META

void Optimizer::basic_vnd()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Local search: Basic VND" << std::endl;
#endif

	bool iter_improved;
	do {
		iter_improved = false;
		for (const auto& operation : this->operation_list) {
			if (operation()) {
				iter_improved = true;
				break;
			}
		}
	} while(iter_improved);
}

void Optimizer::pipe_vnd()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Local search: Pipe VND" << std::endl;
#endif

	bool iter_improved;
	do {
		iter_improved = false;
		for (const auto& operation : this->operation_list) {
			while (operation()) {
				iter_improved = true;
			}
		}
	} while(iter_improved);
}

void Optimizer::random_vnd()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Local search: Random VND" << std::endl;
#endif

	bool iter_improved;
	uint n_op = this->operation_list.size();
	
	std::vector<uint> op_order(n_op);
	for (uint i = 0; i < n_op; i++) {
		op_order[i] = i;
	}

	do {
		iter_improved = false;
		std::random_shuffle(op_order.begin(), op_order.end());

		for (uint i = 0; i < n_op; i++) {
			if (this->operation_list[op_order[i]]()) {
				iter_improved = true;
				break;
			}
		}

	} while(iter_improved);
}

void Optimizer::random_pipe_vnd()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Local search: Random pipe VND" << std::endl;
#endif

	bool iter_improved;
	uint n_op = this->operation_list.size();
	
	std::vector<uint> op_order(n_op);
	for (uint i = 0; i < n_op; i++) {
		op_order[i] = i;
	}

	do {
		iter_improved = false;
		std::random_shuffle(op_order.begin(), op_order.end());

		for (uint i = 0; i < n_op; i++) {
			while (this->operation_list[op_order[i]]()) {
				iter_improved = true;
			}
		}

	} while(iter_improved);
}


void Optimizer::cyclic_vnd()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED
	std::cout << "Local search: Cyclic VND" << std::endl;
#endif

	uint i = 0;
	uint i_impr = 0;
	uint n_op = this->operation_list.size();
	while (true) {
		if (this->operation_list[i]()) {
			i_impr = i;
		}
		i = (i + 1) % n_op;
		if (i == i_impr)
			break;
	}
}

// LOCAL SEARCH OPERATORS
bool Optimizer::insert()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Insert" << std::flush;
#endif
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

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
			assert(Solution::verify_perm_freq(new_perm, new_freq));
#endif

#if defined VERIFY_LBS_UBS && VERIFY_LBS_UBS
			assert(this->instance->is_ub_met(new_freq));
#endif

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

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

bool Optimizer::remove()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Remove" << std::flush;
#endif

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
		// if (i > 0)
			// memcpy(&(new_perm[0]), &(perm[0]), sizeof(uint)*i);
		std::copy(&perm[0], &perm[i], &new_perm[0]);
		// if (i < perm_size - 1)
			// memcpy(&(new_perm[i]), &(perm[i+1]), sizeof(uint)*(perm_size - i - 1));
		std::copy(&perm[i+1], &perm[perm_size], &new_perm[i]);

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
		assert(Solution::verify_perm_freq(new_perm, new_freq));
#endif

#if defined VERIFY_LBS_UBS && VERIFY_LBS_UBS
		assert(this->instance->is_lb_met(new_freq) && 
			   this->instance->is_ub_met(new_freq));
#endif

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

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

bool Optimizer::two_opt()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Two opt" << std::flush;
#endif

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

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
			assert(Solution::verify_perm_freq(new_perm, freq));
#endif


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

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

bool Optimizer::move(uint p, bool reverse)
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Move" << (reverse ? " reverse " : " ") << p << std::flush;
#endif
	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = this->curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;


#pragma omp parallel for default(none) shared(p, reverse, perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i <= perm_size - p; i++) {
		std::vector<uint> new_perm(perm_size, 0);
		std::copy(&perm[0], &perm[i], &new_perm[p]);
		std::copy(&perm[i+p], &perm[perm_size], &new_perm[i+p]);

		for (uint j = 0; j <= perm_size-p; j++) {
			if (i == j) {
				new_perm[j] = new_perm[j+p];
				continue;
			}
			if (reverse)
				std::reverse_copy(&perm[i], &perm[i+p], &new_perm[j]);
			else 
				std::copy(&perm[i], &perm[i+p], &new_perm[j]);

			// assert(Solution::verify_perm_freq(new_perm, freq));
#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
			assert(Solution::verify_perm_freq(new_perm, freq));
#endif

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
					updated = true;
				}
			}

			// repair
			new_perm[j] = new_perm[j+p];
		}
	}

	if (updated)
		this->curr_sol = best_sol;

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

bool Optimizer::exchange(uint p, uint q, bool reverse)
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Exchange" << (reverse ? " reverse " : " ") << p << " " << q << std::flush;
#endif

	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

#pragma omp parallel for default(none) shared(p, q, reverse, perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i <= perm_size - p - q; i++) {
		std::vector<uint> new_perm(perm_size);

		std::copy(&perm[0], &perm[i], &new_perm[0]);
		std::copy(&perm[i + p + q], &perm[perm_size], &new_perm[i + p + q]);
		
		for (uint j = i + p; j + q <= perm_size; j++) {
			uint k = j + q - p;

			if (reverse) {
				std::reverse_copy(&perm[i], &perm[i+p], &new_perm[k]);
				std::reverse_copy(&perm[j], &perm[j+q], &new_perm[i]);
			}
			else {
				std::copy(&perm[i], &perm[i+p], &new_perm[k]);
				std::copy(&perm[j], &perm[j+q], &new_perm[i]);
			}

			assert(Solution::verify_perm_freq(new_perm, freq));

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
					updated = true;
				}
			}
			
			// repair
			new_perm[k] = perm[j];
		}
	}

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	if (updated)
		this->curr_sol = best_sol;

	return updated;
}


bool Optimizer::exchange_nodes()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Exchange nodes" << std::flush;
#endif

	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i < this->instance->node_cnt; i++) {
		std::vector<uint> new_perm = perm;
		std::vector<uint> new_freq = freq;
		
		for (uint j = 0; j < this->instance->node_cnt; j++) {
			if (i == j) 
				continue;
			
			if (freq[i] < this->instance->lbs[j] || freq[i] < this->instance->lbs[j])
				continue;

			if (this->instance->use_ubs)
				if ((freq[i] > this->instance->ubs[j] && this->instance->ubs[j] > 0) ||
					(freq[j] > this->instance->ubs[i] && this->instance->ubs[i] > 0))
						continue;

			for (uint k = 0; k < perm_size; k++) {
				if (perm[k] == i)
					new_perm[k] = j;

				if (perm[k] == j)
					new_perm[k] = i;
			}

			new_freq[i] = freq[j];
			new_freq[j] = freq[i];

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
			assert(Solution::verify_perm_freq(new_perm, new_freq));
#endif

#if defined VERIFY_LBS_UBS && VERIFY_LBS_UBS
			assert(this->instance->is_lb_met(new_freq) && 
				   this->instance->is_ub_met(new_freq));
#endif

			fitness_t new_fitness = 0;
			bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
			{
				if (new_fitness < best_sol.fitness) {
					best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
					updated = true;
				}
			}
			
			// repair
			new_freq[j] = freq[j];
			for (uint k = 0; k < perm_size; k++) {
				if (perm[k] == j)
					new_perm[k] = j;
			}

		}
	}

	if (updated)
		this->curr_sol = best_sol;

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

bool Optimizer::exchange_first_nodes()
{
#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << "Local op: Exchange first nodes" << std::flush;
#endif

	std::vector<uint> perm = this->curr_sol.permutation;
	std::vector<uint> freq = this->curr_sol.frequency;

	Solution best_sol = curr_sol;

	const uint perm_size = perm.size();
	bool updated = false;

#pragma omp parallel for default(none) shared(perm, freq, best_sol, perm_size, updated)
	for (uint i = 0; i < this->instance->node_cnt; i++) {
		
		
		for (uint j = 0; j < this->instance->node_cnt; j++) {
			std::vector<uint> new_perm = perm;
			if (i == j) 
				continue;


			uint k_i = 0;
			uint k_j = 0;

			const uint max_n = std::max(freq[i], freq[j]);

			for (uint n = 1; n <= max_n; n++) {
				for (; perm[k_i] != i; k_i++) assert(k_i < perm_size); // find next k_i
				for (; perm[k_j] != j; k_j++) assert(k_j < perm_size); // find next k_j

				new_perm[k_i] = j;
				new_perm[k_j] = i;

#if defined VERIFY_PERM_FREQ && VERIFY_PERM_FREQ
				assert(Solution::verify_perm_freq(new_perm, freq));
#endif

				fitness_t new_fitness = 0;
				bool new_feasible = this->instance->get_fitness(new_fitness, new_perm, freq);

#pragma omp critical
				{
					if (new_fitness < best_sol.fitness) {
						best_sol = Solution(new_perm, freq, new_feasible, new_fitness);
						updated = true;
					}
				}
			
			// repair

				k_i++;
				k_j++;
			}
		}
	}

	if (updated)
		this->curr_sol = best_sol;

#if defined STDOUT_ENABLED && STDOUT_ENABLED && defined STDOUT_LOCAL_OP && STDOUT_LOCAL_OP
	std::cout << (updated ? " - UPDATED" : " - SAME") << std::endl;
#endif

	return updated;
}

void Optimizer::init_rng(uint seed)
{
	if (seed == 0) {
		std::random_device rd;
		seed = rd();
	}
	this->rng = new std::mt19937(seed);
}

