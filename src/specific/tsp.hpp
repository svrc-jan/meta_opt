#pragma once

#include <assert.h>
#include <iostream>

#include "generic/instance.hpp"
#include "utils.hpp"

class TSP_instance : public Instance 
{
public:
	TSP_instance(const std::string& name, 
		const std::vector<std::vector<uint>>& dist_mtx);
	
	TSP_instance(const std::string& name, const std::string& json_file);

private:
	std::string name;
	std::vector<std::vector<uint>> dist_mtx;

	bool get_conditions_penalty(fitness_t& fitness, 
		const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	fitness_t get_actual_fitness(
		const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	
	void setup_bounds();
	void resize_dist_mtx();

	void parse_json_file(const std::string& file_name);
	void fill_dist_mtx_euc2d(const std::vector<std::vector<double>>& data);
};
