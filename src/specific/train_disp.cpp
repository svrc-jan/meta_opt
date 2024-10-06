#include "train_disp.hpp"

Train_disp_inst::Train_disp_inst(const std::string& name, 
	const std::string& json_file) : name(name)
{
	this->parse_json_file(json_file);
}

void Train_disp_inst::parse_json_file(const std::string& json_file)
{
	json td_json = get_json_file(json_file);

	this->n_res = 0;

	uint train_json_id = 0, op_json_id, op_idx = 0;

	// build indexing and basic data
	for (auto train : td_json["trains"]) {
		op_json_id = 0;
		op_idx_map.push_back(std::vector<uint>());
		for (auto op_json : train) {
			op_idx_map[train_json_id].push_back(op_idx);
			op_json_map.push_back(
				std::pair<uint, uint>(train_json_id, op_json_id));

			Train_op op;
			op.dur = op_json["min_duration"];

			if (op_json.contains("start_lb"))
				op.lb = op_json["start_lb"];

			if (op_json.contains("start_ub"))
				op.lb = op_json["start_ub"];


			// add resource to map
			if (op_json.contains("resources")) {
				for (auto res : op_json["resources"]) {
					std::string res_name = res["resource"];
					if (this->res_idx_map.find(res_name) == this->res_idx_map.end())
						this->res_idx_map[res_name] = this->n_res++;
				}
			}

			this->ops.push_back(op);

			op_json_id++;
			op_idx++;
		}
		train_json_id++;
	}

	// add succesors and resources
	train_json_id = 0;
	op_idx = 0;
	for (auto train : td_json["trains"]) {
		op_json_id = 0;
		for (auto op_json : train) {
			Train_op& op = this->ops[op_idx];

			op.succ.clear();
			for (uint succ : op_json["successors"]) {
				op.succ.push_back(op_idx_map[train_json_id][succ]);
			}

			// add resource to op
			op.res.clear();
			op.release_time.clear();
			if (op_json.contains("resources")) {
				for (auto res : op_json["resources"]) {
					std::string res_name = res["resource"];
					op.res.push_back(this->res_idx_map.at(res_name));

					uint release_time = 0;
					if (res.contains("release_time"))
						release_time = res["release_time"];
					op.release_time.push_back(release_time);
				}
			}

			op_json_id++;
			op_idx++;
		}
		train_json_id++;
	}

	// build predecessor map
	for (uint i = 0; i < this->ops.size(); i++) {
		for (uint succ : this->ops[i].succ) {
			this->ops[succ].pred.push_back(i);
		}
	}
}