#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#pragma once
#pragma once

#include <assert.h>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define VEC_TRUNCATE_SIZE 10

inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

inline json get_json_file(const std::string& file_name)
{
	if (!file_exists(file_name)) {
        std::cerr << "json file '" << file_name << "' not found" << std::endl;
        exit(1);
    }

    std::ifstream file(file_name);
	json config = json::parse(file);

	return config;
}

inline std::vector<std::vector<double>> get_json_mtx_data(json data)
{
	std::vector<std::vector<double>> mtx;
	for (auto row : data) {
		mtx.push_back(std::vector<double>());
		for (auto el : row) {
			mtx.back().push_back(el);
		}
	}
	return mtx;
}

inline double euc_dist(const std::vector<double>& a, const std::vector<double>& b)
{
	assert(a.size() == b.size());
	
	double sum = 0;
	for (size_t i = 0; i < a.size(); i++) {
		sum += pow(a[i] - b[i], 2);
	}

	return sqrt(sum);
}

template<typename T>
inline void print_vec(std::ostream& os, const std::vector<T>& vec)
{
	os << "[";
	for (size_t i = 0; i < vec.size(); i++) {
		os << vec[i] << (i < vec.size() - 1 ? ", " : "");

#ifdef VEC_TRUNCATE_SIZE
		if (i >= VEC_TRUNCATE_SIZE) {
			os << "...";
			break;
		}
#endif

	}
	os << "]";
}

inline json get_case_insensitive_json(json& in_json, const std::string& key_in)
{
	using namespace std;
	string key = key_in;
	if (in_json.contains(key)) {
		return in_json[key];
	}
	std::transform(key.begin(), key.end(), key.begin(), ::toupper);
	if (in_json.contains(key)) {
		return in_json[key];
	}

	cerr << "Incorrect JSON key " << key << endl;
	exit(101);
}

#endif