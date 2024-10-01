#include "solution.hpp"


Solution::Solution(uint node_count) :
	frequency(std::vector<uint>(node_count, 0)),
	feasible(false), 
	fitness(MAX_FITNESS / 2) { }

Solution::Solution(Solution& sol) :
	permutation(sol.permutation),
	frequency(sol.frequency),
	feasible(sol.feasible),
	fitness(sol.fitness) { }


Solution::Solution(std::vector<uint>& perm, std::vector<uint>& freq, 
        bool& feasible, fitness_t& fitness) :
	permutation(perm),
	frequency(freq),
	feasible(feasible),
	fitness(fitness) { }


void Solution::print(std::ostream& os)
{
	os << "Solution [";

	for (uint i = 0; i < this->size(); i++) {
		os << this->permutation[i] << ( i != this->size() - 1 ? ", " : "");
	};
	os << "], fitness: " << this->fitness << ", " << (this->feasible ? "feasible" : "infeasible");
}

std::ostream& operator << (std::ostream& os, Solution& sol)
{
	sol.print(os);
	return os;
}