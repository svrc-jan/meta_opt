#pragma once

#include <assert.h>
#include <iostream>

#include "generic/instance.hpp"

class TSP_instance : public Instance 
{
public:
	TSP_instance(const std::string& name, 
		std::vector<std::vector<uint>> distance_matrix);

	TSP_instance(TSP_instance& inst);

private:
	std::string name;
	std::vector<std::vector<uint>> distance_matrix;

	fitness_t get_conditions_penalty(const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);

	fitness_t get_actual_fitness(const std::vector<uint>& permutation,
		const std::vector<uint>& frequency);
};
