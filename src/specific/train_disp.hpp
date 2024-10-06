#pragma once

#include <assert.h>
#include <iostream>
#include <utility>

#include "generic/instance.hpp"
#include "utils.hpp"

struct Train_op {
	uint dur = 0;

	std::vector<uint> succ;
	std::vector<uint> pred;

	uint res_cnt = 0;
	std::vector<uint> res;
	std::vector<uint> release_time;

	uint lb = 0;
	uint ub = std::numeric_limits<uint>::max();

	uint threshold = std::numeric_limits<uint>::max();
	uint coeff = 0;
	uint increment = 0;
};


class Train_disp_inst : public Instance
{
public:
	std::string name;

	Train_disp_inst(const std::string& name, 
	const std::string& json_file);

private:
	// instance data
	std::vector<std::vector<uint>> op_idx_map;
	std::vector<std::pair<uint, uint>> op_json_map;
	
	std::map<std::string, uint> res_idx_map;
	uint n_res;

	std::vector<Train_op> ops;

	void parse_json_file(const std::string& file_name);
};
