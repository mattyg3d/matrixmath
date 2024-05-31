#include "Matrix.h"

Matrix::Matrix(size_t rows, size_t colms)
	: rows(rows)
	, colms(colms)
{
	// and sets their values to 0
	this->vals = new float* [this->rows];
	for (size_t i = 0; i < this->rows; i++) {
		this->vals[i] = new float[this->colms];
	}
}

Matrix::Matrix(const Matrix& other)
	: rows(other.rows)
	, colms(other.colms)
{
	this->vals = new float* [this->rows];
	for (size_t i = 0; i < this->rows; i++) {
		this->vals[i] = new float[this->colms];
	}

	for (size_t i = 0; i < this->rows; ++i) {
		for (size_t j = 0; j < this->colms; ++j) {
			this->vals[i][j] = other.vals[i][j];
		}
	}
}

Matrix::Matrix(std::vector<std::vector<float>> vals)
{
	if (vals.empty()) {
		throw std::invalid_argument("Input vector is empty.");
	}

	this->rows = vals.size();
	this->colms = vals[0].size();

	// check to ensure all rows are filled to correct capacity of columns
	for (const auto& row : vals) {
		const size_t currentRowColmns = row.size();

		if (currentRowColmns != this->colms) {
			throw std::invalid_argument("All rows must have the same number of columns.");
		}
	}
	
	// creates array with defined row and column sizes
	this->vals = new float*[this->rows];
	for (size_t i = 0; i < this->rows; i++) {
		this->vals[i] = new float[this->colms];
	}
	
	// copies values of inputted vector to the array
	for (size_t i = 0; i < this->rows; ++i) {
		for (size_t j = 0; j < this->colms; ++j) {
			this->vals[i][j] = vals[i][j];
		}
	}
}

Matrix::~Matrix()
{
	for (size_t i = 0; i < this->rows; i++) {
		delete[] this->vals[i];
	}

	delete[] this->vals;
}

bool Matrix::isInvertible() const
{
	return false;
}

bool Matrix::isConsistent() const
{
	return false;
}

bool Matrix::isDependent() const
{
	return false;
}

size_t Matrix::getRows() const
{
	return this->rows;
}

size_t Matrix::getColms() const
{
	return this->colms;
}

void Matrix::setRows(size_t newRows)
{
	this->rows = newRows;
}

void Matrix::setColms(size_t newColms)
{
	this->colms = newColms;
}

void Matrix::addWith(const Matrix& other)
{
	if ((this->rows != other.getRows()) || (this->colms != other.getColms()))
		throw std::invalid_argument("All matrices must have the same dimensions.");

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->colms; j++)
		{
			this->vals[i][j] += other.vals[i][j];
		}
	}
}

Matrix Matrix::add(const Matrix& other) const
{
	// 3 approaches:
	// 1. make a std::vector<std::vector<float>> array with the sum of both, then return Matrix(array)
	// 2. make a blank Matrix object w/ same dimensions, then iterate self and other vals and set new matrix's position to be a + b
	// 3. make a copy of self and then do copy.addWith(other) and return copy
	
	if ((this->rows != other.getRows()) || (this->colms != other.getColms()))
		throw std::invalid_argument("All matrices must have the same dimensions.");

	Matrix sumMatrix(this->rows, this->colms);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->colms; j++)
		{
			sumMatrix.vals[i][j] = this->vals[i][j] + other.vals[i][j];
		}
	}

	return sumMatrix;
}

Matrix Matrix::mult(const Matrix& other) const
{
	if (this->colms != other.getRows())
		throw std::invalid_argument("The number of columns in the first matrix must be equal to the number of rows in the second matrix.");

	Matrix productMatrix(this->rows, other.getColms());

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < other.getColms(); j++)
		{
			float sum = 0;

			const size_t sizeOfThisColmsAndOtherRows = this->colms;

			for (int k = 0; k < sizeOfThisColmsAndOtherRows; k++)
			{
				sum += this->vals[i][k] * other.vals[k][j];
			}

			productMatrix.vals[i][j] = sum;
		}
	}

	return productMatrix;
}

void Matrix::cMultWith(float coef)
{
	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->colms; j++)
		{
			this->vals[i][j] *= coef;
		}
	}
}

Matrix Matrix::cMult(float coef) const
{
	Matrix productMatrix = Matrix(*this);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < this->colms; j++)
		{
			productMatrix.vals[i][j] *= coef;
		}
	}

	return productMatrix;
}

void Matrix::printMatrix() const
{
	for (int i = 0; i < this->rows; i++)
	{
		std::cout << "| ";

		for (int j = 0; j < this->colms; j++)
		{
			std::cout << this->vals[i][j] << " ";
		}

		std::cout << "|" << std::endl;
	}
}

Matrix Matrix::getSteadyState(const Matrix& transitionMatrix, float epsilon = 0.001) const
{
	if (this->rows != 1)
		throw std::invalid_argument("Matrix must be a row matrix.");

	if (this->colms != transitionMatrix.getRows())
		throw std::invalid_argument("The number of columns in the first matrix must be equal to the number of rows in the second matrix.");

	Matrix currentState = Matrix(*this);
	Matrix nextState = currentState.mult(transitionMatrix);

	while (true)
	{
		currentState = nextState;
		nextState = currentState.mult(transitionMatrix);

		bool isSteadyState = true;

		for (int i = 0; i < currentState.getColms(); i++)
		{
			if (abs(currentState.vals[0][i] - nextState.vals[0][i]) > epsilon)
			{
				isSteadyState = false;
				break;
			}
		}

		if (isSteadyState)
		{
			break;
		}
	}

	return currentState;
}


float Matrix::getDeterminant()
{
	if (this->rows != this->colms)
		throw std::invalid_argument("To find the determinant of a matrix, it must be square.");

	Matrix temp(*this);

	for (int i = 0; i < temp.getRows(); i++) 
	{
		for (int j = i + 1; j < temp.getRows(); j++) 
		{
			float multiplier = temp.vals[j][i] / temp.vals[i][i];
			for (int k = i; k < temp.getRows(); k++) 
			{
				temp.vals[j][k] -= multiplier * temp.vals[i][k];
			}
		}
	}

	float product = 1;

	for (int i = 0; i < temp.getRows(); i++)
	{
		product *= temp.vals[i][i];
	}
	
	return product;
}

void Matrix::toTranspose()
{
	Matrix temp(this->colms, this->rows);

	for (int i = 0; i < temp.getRows(); i++)
	{
		for (int j = 0; j < temp.getColms(); j++)
		{
			temp.vals[i][j] = this->vals[j][i];
		}
	}

	*this = temp;

}

//void Matrix::toRef() const
//{
//	if ((this->rows == 1) || (this->colms == 1))
//		throw std::invalid_argument("Please no.");
//
//	for (int i = 0; i < this->rows; i++)
//	{
//
//	}
//}
//
//void Matrix::toRref() const
//{
//
//}
//
//Matrix Matrix::giveRef() const
//{
//
//}
//
////void Matrix::cipherWith(Matrix cipherMatrix) const
////{
////	if()
////}
////
////Matrix Matrix::cipher(Matrix cipherMatrix) const
////{
////
////	return;
////}
//