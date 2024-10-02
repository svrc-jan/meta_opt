#pragma once

#include <assert.h>
#include <iostream>

#include "generic/instance.hpp"
#include "utils.hpp"

class TSP_instance : public Instance 
{
public:
	TSP_instance(const std::string& name, const Matrix<uint>& distance_matrix);
	TSP_instance(const std::string& name, const std::string& json_file);

	TSP_instance(TSP_instance& inst);

private:
	std::string name;
	// std::vector<std::vector<uint>> distance_matrix;
	Matrix<uint> distance_matrix;

	bool get_conditions_penalty(fitness_t& fitness, const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	fitness_t get_actual_fitness(const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	void parse_json_file(const std::string& file_name);

	void setup_bounds();
};
