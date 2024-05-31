#include "Matrix.h"


int main(int argc, char** argv)
{

	Matrix a({ {15,15}, {-0.2, -94.52} });

	const Matrix b({ {.2,.8},{.6,.4} });

	std::cout << "a:\n" << a << std::endl;
	std::cout << "\nb:\n" << b << std::endl;

	a += b;

	std::cout << "\na:\n" << a << std::endl;

	Matrix c = a * b;

	std::cout << "\nc:\n" << c << std::endl;

	return 0;
}