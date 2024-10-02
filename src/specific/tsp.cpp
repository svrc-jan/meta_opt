#include "tsp.hpp"

TSP_instance::TSP_instance(const std::string& name, 
	const std::vector<std::vector<uint>>& dist_mtx) :
	Instance(name), dist_mtx(dist_mtx)
{
	this->node_cnt = dist_mtx.size();
	for (auto v : dist_mtx) {
		assert(v.size() == this->node_cnt);
	}

	this->setup_bounds();
}

TSP_instance::TSP_instance(const std::string& name, 
	const std::string& json_file) : Instance(name)
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

void TSP_instance::resize_dist_mtx()
{
	this->dist_mtx.clear();
	
	for (uint i = 0; i < this->node_cnt; i++)
		this->dist_mtx.push_back(std::vector<uint>(this->node_cnt, 0));
}

TSP_instance::TSP_instance(TSP_instance& inst) : 
	Instance(inst),	dist_mtx(inst.dist_mtx) {}


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

	for (uint i = 0; i < perm_size; i++)
		dist += dist_mtx[permutation[i]][permutation[(i < perm_size - 1 ? i + 1 : 0)]];
	
	return dist;
}

void TSP_instance::parse_json_file(const std::string& file_name)
{
	json tsp_json = get_json_file(file_name);
	
	assert(std::string(get_case_insensitive_json(tsp_json, "type")) == "TSP");

	this->node_cnt = get_case_insensitive_json(tsp_json, "dimension");

	this->resize_dist_mtx();


	std::string edge_weight_type = get_case_insensitive_json(tsp_json, "edge_weight_type");

	if (edge_weight_type == "EUC_2D") {
		auto data_json = get_case_insensitive_json(tsp_json, "node_coordinates");
		auto node_coordinates = get_json_mtx_data(data_json);
		this->fill_dist_mtx_euc2d(node_coordinates);
	}
	else {
		std::cerr << "incorrect edge type" << std::endl;
		exit(200);
	}
}



void TSP_instance::fill_dist_mtx_euc2d(const std::vector<std::vector<double>>& data)
{
	for (uint i = 0; i < this->node_cnt; i++) {
		for (uint j = 0; j < this->node_cnt; j++) {
			this->dist_mtx[i][j] = (uint)round(euc_dist(data[i], data[j]));
		}
	}
}