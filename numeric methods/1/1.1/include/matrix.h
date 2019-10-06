#ifndef VECTOR
#define VECTOR

class Matrix {
public:
	vector <vector <double>> x;
	int n, m;
	vector <double>& operator[](int i);
	Matrix(int nn, int mm);
	void Matrix::readMatrix(string &&path);
	void show();
	int size();
	void transpose();
	double norm();
};

#endif
