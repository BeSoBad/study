#ifndef MATRIX
#define MATRIX

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

class Matrix {
public:
	std::vector <std::vector <double>> x;
	int n, m;
	std::vector <double>& operator[](int i);
	Matrix() {

	}
	Matrix(int nn, int mm);
	void readMatrix(std::string &&path);
	void show();
	int size();
	void transpose();
	double norm();
};

#endif
