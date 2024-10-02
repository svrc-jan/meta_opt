#pragma once

#include <string>
#include <vector>
#include <numeric>
#include <iostream>

#include "utils.hpp"
#include "config.hpp"

class Instance
{
public:
	Instance(const std::string& name, const uint node_cnt, 
		const uint common_lb=0, const uint common_ub=0);
	
	Instance(const std::string& name, const std::vector<uint> node_lb, 
		const std::vector<uint> node_ub);

	Instance(const Instance& inst);

	Instance(const std::string& name="") : name(name), node_cnt(0), use_ubs(false) {}

	std::string name;
	uint node_cnt;
	
	std::vector<uint> lbs;
	std::vector<uint> ubs;
	bool use_ubs;


	void print(std::ostream& os);

	bool get_fitness(fitness_t& fitness, const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	fitness_t get_lb_penalty(const std::vector<uint>& frequency);
	fitness_t get_ub_penalty(const std::vector<uint>& frequency);

	bool is_lb_met(const std::vector<uint>& frequency);

	virtual bool get_conditions_penalty(fitness_t& fitness, const std::vector<uint>& permutation,
		const std::vector<uint>& frequency) { return true; }

	virtual fitness_t get_actual_fitness(const std::vector<uint>& permutation,
		const std::vector<uint>& frequency) { return 0; }

};
