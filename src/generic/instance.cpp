#include "instance.hpp"

Instance::Instance(const std::string& name, const uint node_cnt, 
	const uint common_lb, const uint common_ub) : name(name), node_cnt(node_cnt)
{
	// if upper bound is 0, dont enforce
	use_ubs = common_ub > 0;

	this->lbs.resize(node_cnt);
	if (use_ubs)
		this->ubs.resize(node_cnt);

	for (uint i = 0; i < node_cnt; i++) {
		this->lbs[i] = common_lb;
		if (use_ubs) {
			this->ubs[i] = common_ub;
		}
	}
}

Instance::Instance(const std::string& name, const std::vector<uint> node_lbs, 
	const std::vector<uint> node_ubs) : name(name)
{
	assert(node_lbs.size() == node_ubs.size() || node_ubs.size() == 0);
	this->node_cnt = node_lbs.size();
	this->use_ubs = false;

	for (uint i = 0; i < this->node_cnt; i++) {
		this->lbs[i] = node_lbs[i];
		if (node_ubs.size() > 0) {
			this->ubs[i] = node_ubs[i];
			if (node_ubs[i] > 0)
				this->use_ubs = true;
		}
	}
}

Instance::Instance(const Instance& inst) :
	name(inst.name),
	node_cnt(inst.node_cnt),
	lbs(inst.node_cnt),
	ubs(inst.ubs),
	use_ubs(inst.use_ubs) { }

bool Instance::get_fitness(fitness_t& fitness, const std::vector<uint>& permutation,
		const std::vector<uint>& frequency)
{
	fitness = this->get_actual_fitness(permutation, frequency);
	return this->get_conditions_penalty(fitness, permutation, frequency);
}

fitness_t Instance::get_lb_penalty(const std::vector<uint>& frequency)
{
	uint count = 0;
	for (uint i = 0; i < this->node_cnt; i++) {
		count += (uint)std::max((int)this->lbs[i] - (int)frequency[i], 0);
	}

	return LB_PENALTY*count;
}

fitness_t Instance::get_ub_penalty(const std::vector<uint>& frequency)
{
	if (!this->use_ubs)
		return 0;
	
	uint count = 0;
	for (uint i = 0; i < this->node_cnt; i++) {
		if (this->ubs[i] > 0)
			count += (uint)std::max((int)frequency[i] - (int)this->ubs[i], 0);
	}

	return UB_PENALTY*count;
}

bool Instance::is_lb_met(const std::vector<uint>& frequency)
{
	for (uint i = 0; i < this->node_cnt; i++) {
		if (this->lbs[i] > frequency[i])
			return false;
	}
	return true;
}

bool Instance::is_ub_met(const std::vector<uint>& frequency)
{
	if (!this->use_ubs)
		return true;
	
	for (uint i = 0; i < this->node_cnt; i++) {
		if (this->lbs[i] < frequency[i] && this->ubs[i] > 0)
			return false;
	}
	return true;
}

void Instance::print(std::ostream& os)
{
	os << "Instance '" << this->name << "', ";
	os << "node count: " << this->node_cnt  << ", ";
	os << "lbs: ";
	print_vec<>(os, this->lbs);
	if (this->use_ubs) {
		os << ", ubs ";
		print_vec<>(os, this->ubs);
	}
}