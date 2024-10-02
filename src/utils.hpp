#pragma once

#include <assert.h>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

template <typename T>
class Matrix
{
public:	
	uint rows;
	uint cols;

	T* data = nullptr;

	Matrix(uint rows=0, uint cols=0) : rows(rows), cols(cols) 
	{
		if (rows > 0 && cols > 0)
			this->data = new T[rows*cols];
	}

	Matrix(const Matrix& mtx) : rows(mtx.rows), cols(mtx.cols) 
	{
		if (mtx.data != nullptr)
			this->data = new T[this->rows*this->cols];
			memcpy(this->data, mtx.data, sizeof(T)*rows*cols);
	}

	Matrix(std::vector<std::vector<T>> mtx) {
		this->rows = mtx.size();
		this->cols = (this->rows > 0) ? mtx[0].size() : 0;

		if (rows > 0 && cols > 0) {
			this->data = new T[this->rows*this->cols];
			for (uint i = 0; i < this->rows; i++) {
				assert(mtx[i].size() == this->cols);
				for (uint j = 0; j < this->cols; j++) {
					this->set(i, j, mtx[i][j]);
				}
			}
		}
	}

	~Matrix()
	{
		this->clear();
	}

	void resize(uint new_rows, uint new_cols)
	{
		T* new_data = new T[rows*cols];

		if (this->data != nullptr) {
			for (uint i = 0; i < std::min(new_rows, this->rows); i++) {
				for (uint j = 0; j < std::min(new_cols, this->cols); j++) {
					new_data[new_cols*i + j] = this->get(i, j);
				}
			}
			delete[] this->data;
		}

		this->data = new_data;
		this->rows = new_rows;
		this->cols = new_cols;
	}

	void clear()
	{
		if (this->data != nullptr)
			delete[] this->data;
			this->data = nullptr;
		this->rows = 0;
		this->cols = 0;
	}

	T get(const uint row, const uint col) 
	{ 
		assert(row < this->rows && col < this->rows);
		return this->data[this->idx(row, col)];
	}
	
	void set(const uint row, const uint col, const T& val) 
	{
		assert(row < this->rows && col < this->rows);
		this->data[this->idx(row, col)] = val;
	}

	inline uint idx(uint row, uint col) 
	{
		return this->cols*row + col;
	}
};
