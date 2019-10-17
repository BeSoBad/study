#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "vector.h"
#include "operations.h"

using namespace std;

double end(Matrix& A) {
	double sum = 0, size = A.size();
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
			sum += A[i][j]*A[i][j];
	return std::sqrt(sum);
}

void max_elem(Matrix& A, int& max_i, int& max_j) {
	int max = A[0][0], size = A.size();
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; i++) {
			if (std::abs(A[i][j]) > max) {
				max = std::abs(A[i][j]);
				max_i = i;
				max_j = j;
			}
		}
	}
}

double calc_phi(double a_ii, double a_ij, double a_jj) {
	if (a_ii == a_jj)
		return M_PI / 4;
	else
		return 0.5*std::atan(2*a_ij/(a_ii - a_jj));
}


void jacobi(Matrix& A, double eps, Matrix& evectors, Vector& evalues) {
	int size = A.size();
	Matrix A_i = A;
	Matrix ev(size, size);
	while (1) {
		Matrix U(size, size);
		int i, j;
		max_elem(A_i, i, j);
		double phi = calc_phi(A_i[i][i], A_i[i][j], A_i[j][j]);
		U[i][j] = -std::sin(phi);
		U[j][i] = std::sin(phi);
		U[i][i] = std::cos(phi);
		U[j][j] = std::cos(phi);
		Matrix U_T = U;
		U_T.transpose();
		Matrix mtmp = U_T*A_i;
		A_i = mtmp;
		Matrix mtmp2 = ev*U;
		ev = mtmp;
		if (end(A_i) < eps)
			break;
	}
	for (int i = 0; i < size; i++)
		evalues[i] = A_i[i][i];
}

int main() {
	std::string path = "matrix.txt";
	Matrix A;
	A.readMatrix(path);
	Vector b;
	b.readVector("vector.txt");
	
	double eps;
	std::cin >> eps;

	Matrix evectors(A.size(), A.size());
	Vector evalues(A.size());
	jacobi(A, eps, evectors, evalues);;
	evectors.show();
	evalues.show();
    return 0;
}