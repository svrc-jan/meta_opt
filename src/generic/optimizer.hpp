#pragma once

#include <vector>
#include <cstring>
#include <random>

#include "config.hpp"
#include "generic/instance.hpp"
#include "generic/solution.hpp"


class Optimizer
{
public:
	Optimizer(Instance* instance, uint seed=0);
	~Optimizer();

	Solution get_best_sol() { return this->best_known_sol; }

	void set_operation_list(std::vector<std::string> operation_names);
	void set_all_operation_list();
	void set_perturbation(std::string perturbation_name);
	void set_local_search(std::string local_search_name);
	void set_metaheuristic(std::string metaheuristic_name);
	void run() { this->metaheuristic(); }

private:
	Instance* instance;

	Solution init_sol;
	Solution curr_sol;
	Solution best_known_sol;

	std::vector<std::function<bool()>> operation_list;
	std::vector<std::string> operation_names;

	std::function<void(uint)> perturbation = [](uint k){};
	std::string perturbation_name = "";

	std::function<void()> local_search = [](){};
	std::string local_search_name = "";

	std::function<void()> metaheuristic = [](){};
	std::string metaheuristic_name = "";

	std::mt19937* rng;
	void init_rng(uint seed);

	Solution get_naive_solution();
	Solution get_greedy_solution();
	
	// utils
	
	// metaheuristics
	void ils();

	// permutations
	void double_bridge(uint k, bool reverse_all);
	
	// local search meta
	void basic_vnd();
	void pipe_vnd();
	void random_vnd();
	void random_pipe_vnd();
	void cyclic_vnd();

	// local search operators
	bool insert();
	bool remove();
	bool two_opt();
	bool move(uint p, bool reverse);
	bool exchange(uint p, uint q, bool reverse);
	bool exchange_nodes();
	bool exchange_first_nodes();


	// string maps to methods
	const std::map<std::string, std::function<void()>> metaheuristic_map = {
		{"ils", [this](){ this->ils(); }}
	};

	const std::map<std::string, std::function<void()>> local_search_map = {
		{"basic", [this](){ this->basic_vnd(); }},
		{"pipe", [this](){ this->pipe_vnd(); }},
		{"random", [this](){ this->random_vnd(); }},
		{"random_pipe", [this](){ this->random_pipe_vnd(); }},
		{"cyclic", [this](){ this->cyclic_vnd(); }}
	};

	const std::map<std::string, std::function<void(uint)>> perturbation_map = {
		{"double_bridge", [this](uint k){ this->double_bridge(k, false); }},
		{"double_bridge_all", [this](uint k){ this->double_bridge(k, true); }}
	};

	const std::map<std::string, std::function<bool()>> operation_map = {
		{"insert", [this](){ return this->insert(); }},
		{"remove", [this](){ return this->remove(); }},
		{"two_opt", [this](){ return this->two_opt(); }},
		{"exchange_nodes", [this](){ return this->exchange_nodes(); }},
		{"exchange_first_nodes", [this](){ return this->exchange_first_nodes(); }},

		{"move_1", [this](){ return this->move(1, false); }},
		{"move_2", [this](){ return this->move(2, false); }},
		{"move_3", [this](){ return this->move(3, false); }},
		{"move_4", [this](){ return this->move(4, false); }},
		{"move_5", [this](){ return this->move(5, false); }},

		{"move_rev_2", [this](){ return this->move(2, true); }},
		{"move_rev_3", [this](){ return this->move(3, true); }},
		{"move_rev_4", [this](){ return this->move(4, true); }},
		{"move_rev_5", [this](){ return this->move(5, true); }},

		{"exchange_1_1", [this](){ return this->exchange(1, 1, false); }},
		{"exchange_1_2", [this](){ return this->exchange(1, 2, false); }},
		{"exchange_2_1", [this](){ return this->exchange(2, 1, false); }},
		{"exchange_2_2", [this](){ return this->exchange(2, 2, false); }},
		{"exchange_2_3", [this](){ return this->exchange(2, 3, false); }},
		{"exchange_3_2", [this](){ return this->exchange(3, 2, false); }},
		{"exchange_3_3", [this](){ return this->exchange(3, 3, false); }},
		{"exchange_2_4", [this](){ return this->exchange(2, 4, false); }},
		{"exchange_4_2", [this](){ return this->exchange(4, 2, false); }},
		{"exchange_3_4", [this](){ return this->exchange(3, 4, false); }},
		{"exchange_4_3", [this](){ return this->exchange(4, 3, false); }},
		{"exchange_4_4", [this](){ return this->exchange(4, 4, false); }},

		{"exchange_rev_2_1", [this](){ return this->exchange(2, 1, true); }},
		{"exchange_rev_1_2", [this](){ return this->exchange(1, 2, true); }},
		{"exchange_rev_2_2", [this](){ return this->exchange(2, 2, true); }},
		{"exchange_rev_2_3", [this](){ return this->exchange(2, 3, true); }},
		{"exchange_rev_3_2", [this](){ return this->exchange(3, 2, true); }},
		{"exchange_rev_3_3", [this](){ return this->exchange(3, 3, true); }},
		{"exchange_rev_2_4", [this](){ return this->exchange(2, 4, true); }},
		{"exchange_rev_4_2", [this](){ return this->exchange(4, 2, true); }},
		{"exchange_rev_3_4", [this](){ return this->exchange(3, 4, true); }},
		{"exchange_rev_4_3", [this](){ return this->exchange(4, 3, true); }},
		{"exchange_rev_4_4", [this](){ return this->exchange(4, 4, true); }}
	};
};