#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "vector.h"
#include "matrix.h"
#include "operations.h"

using namespace std;

void LU(Matrix A, Matrix &L, Matrix &U, int n) {
	U=A;
	for(int i = 0; i < n; i++)
		for(int j = i; j < n; j++)
			L[j][i] = U[j][i]/U[i][i];

	for(int k = 1; k < n; k++)
	{
		for(int i = k-1; i < n; i++)
			for(int j = i; j < n; j++)
				L[j][i] = U[j][i]/U[i][i];

		for(int i = k; i < n; i++)
			for(int j = k-1; j < n; j++)
				U[i][j] = U[i][j] - L[i][k-1]*U[k-1][j];
	}
}

Vector solve(Matrix& L, Matrix& U, Vector& b) {
	int n = b.size();
	Vector z(n);
	z[0] = b[0];
	for (int i = 1; i < n; i++) {
		z[i] = b[i];
		for (int j = 0; j < i; j++) {
			z[i] -= L[i][j]*z[j];
		}
	}
	Vector x(n);
	x[n - 1] = z[n - 1]/U[n - 1][n - 1];
	for (int i = n - 1; i >= 0; i--) {
		x[i] = z[i];
		for (int j = i + 1; j < n; j++) {
			x[i] -= U[i][j]*x[j];
		}
		x[i] /= U[i][i];
	}
	return x;
}

double determinant(Matrix& L, Matrix& U) {
	double detL = 1, detU = 1;
	for (int i = 0; i < L.size(); i++) {
		detL *= L[i][i];
		detU *= U[i][i];
	}
	return detL*detU;
}

Matrix inverse(Matrix& L, Matrix& U) {
	int n = L.size();
	Vector e(n);
	Matrix IAT(n, n);
	for (int i = 0; i < n; i++) {
		e[i] = 1;
		Vector v = solve(L, U, e);
		IAT[i] = v.x;
	}
	IAT.transpose();
	return IAT;
}
