#include "utils.hpp"


using namespace std;

int main(int argc, char const *argv[])
{
	json tsp_json = get_json_file("../data/tsplib/a280.json");

	// int dimension = tsp_json["dimension"];
	auto n_coord = tsp_json["node_coordinates"];

	vector<vector<double>> data;

	for (auto r : n_coord) {
		data.push_back(vector<double>());
		cout << r << endl;
		for (auto x : r) {
			data.back().push_back(x);
		}
	}
	
	
	return 0;
}

