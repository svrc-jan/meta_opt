#include "tsp.hpp"

TSP_instance::TSP_instance(const std::string& name, 
	const Matrix<uint>& distance_matrix) :
	Instance(name), distance_matrix(distance_matrix)
{
	this->node_cnt = distance_matrix.rows;
	assert(distance_matrix.rows == distance_matrix.cols);

	this->setup_bounds();
}

TSP_instance::TSP_instance(const std::string& name, const std::string& json_file) : Instance(name)
{
	this->parse_json_file(json_file);
	this->setup_bounds();
}

void TSP_instance::setup_bounds()
{
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
		uint d = distance_matrix.get(permutation[i], permutation[(i + 1) % perm_size]); 
		dist += d;
	}
	
	return dist;
}

void TSP_instance::parse_json_file(const std::string& file_name)
{
	json tsp_json = get_json_file(file_name);
	
	assert(std::string(tsp_json["type"]) == "TSP");

	this->node_cnt = tsp_json["dimension"];

	this->distance_matrix.resize(this->node_cnt, this->node_cnt);

	std::vector<double> coord_x;
	std::vector<double> coord_y;

	auto n_coord = tsp_json["node_coordinates"];



	bool is_x = true;
	for (auto r : n_coord) {
		for (auto x : r) {
			double d = x;
			if (is_x)
				coord_x.push_back(d);
			else
				coord_y.push_back(d);
			is_x = !is_x;
		}
	}


	for (uint i = 0; i < this->node_cnt; i++) {
		for (uint j = 0; j < this->node_cnt; j++) {

			double dist = sqrt(pow(coord_x[i] - coord_x[j], 2) + pow(coord_x[i]- coord_x[j], 2));
			this->distance_matrix.set(i, j, (uint)round(dist));
		}
	}

}