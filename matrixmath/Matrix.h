#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

class Matrix
{
private:
	size_t rows;
	size_t colms;

public:
	float** vals;

	Matrix() = delete;
	Matrix(const Matrix& other); // copy constructor
	Matrix(size_t rows, size_t colms);
	Matrix(std::vector<std::vector<float>> vals);
	~Matrix();

	bool isInvertible() const;
	bool isConsistent() const;
	bool isDependent() const;

	size_t getRows() const;
	size_t getColms() const;
	void setRows(size_t newRows);
	void setColms(size_t newColms);

	Matrix add(const Matrix& other) const;
	Matrix operator+(const Matrix& other) const;

	void addWith(const Matrix& other);
	void operator+=(const Matrix& other);

	Matrix mult(float coef) const;
	Matrix operator*(float coef) const;

	Matrix mult(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;

	void multWith(float coef);
	void operator*=(float coef);

	void printMatrix() const;

	Matrix getSteadyState(const Matrix& transitionMatrix, float epsilon) const;

	//void cipherWith(Matrix cipherMatrix);
	//Matrix cipher(Matrix cipherMatrix) const;
	float getDeterminant();

	Matrix transpose() const;

	friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);


	//
	//void toRef() const;
	//Matrix giveRef() const;
	//
	//Matrix inverse(const Matrix& other) const;
};