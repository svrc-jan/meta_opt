#include "tsp.hpp"

TSP_instance::TSP_instance(const std::string& name, 
	std::vector<std::vector<uint>> distance_matrix) :
	Instance(name), distance_matrix(distance_matrix)
{
	this->node_cnt = distance_matrix.size();
	for (uint i = 0; i < this->node_cnt; i++) {
		assert(distance_matrix[i].size() == this->node_cnt);
	}

	this->lbs.resize(this->node_cnt);
	this->ubs.resize(this->node_cnt);
	this->use_ubs = true;

	for (uint i = 0; i < this->node_cnt; i++) {
		this->lbs[i] = 1;
		this->ubs[i] = 1;
	}
}

TSP_instance::TSP_instance(TSP_instance& inst) : 
	Instance(inst),	distance_matrix(inst.distance_matrix) {}


bool TSP_instance::get_conditions_penalty(fitness_t& fitness, 
	const std::vector<uint>& permutation,
	const std::vector<uint>& frequency)
{
	return true;
}

fitness_t TSP_instance::get_actual_fitness(
	const std::vector<uint>& permutation,
	const std::vector<uint>& frequency)
{
	uint dist = 0;

	uint perm_size = permutation.size();

	for (uint i = 0; i < perm_size; i++) {
		uint d = distance_matrix[permutation[i]][permutation[(i + 1) % perm_size]]; 
		dist += d;
	}
	
	return dist;
}