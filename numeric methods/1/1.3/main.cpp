#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "vector.h"
#include "operations.h"


using namespace std;

//----From LU.cpp---//

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

//------------//

void toEquivalentForm(Matrix& A, Vector& b, Matrix& Alpha, Vector& beta) {
	int n = A.size();
	for (int i = 0; i < n; i++) {
		beta[i] = b[i]/(A[i][i] + 1e-3);
		for (int j = 0; j < n; j++)
			if (i != j)
				Alpha[i][j]  = -A[i][j]/(A[i][i] + 1e-3);
	}
}

double finish_iter(Vector x, Vector x_prev, double norm_alpha, double norm_C, double eps) {
	Vector vec = x - x_prev;
	double norm_vec = vec.norm();
	if (norm_alpha == 1)
		return norm_vec <= eps;
	else {
		double coef = norm_C / (1 - norm_alpha);
		return coef * norm_vec <= eps;
	}
}

Vector iterative(Matrix& Alpha, Vector& beta, double eps) {
	Vector x = beta;
	double alpha_norm = Alpha.norm();
	Vector xi(Alpha.size());
	while (1) {
		Vector tmp = Alpha*x;
		xi = beta + tmp;
		if (finish_iter(xi, x, alpha_norm, alpha_norm, eps))
			break;
		x = xi;
	}
	return xi;
}

Vector seidel(Matrix& Alpha, Vector& beta, double eps) {
	int n = Alpha.size();
	Matrix L(n, n), U(n, n);
	LU(Alpha, L, U, n);
	Matrix IAlpha = inverse(L, U);
	Matrix B = IAlpha;
	Matrix C = Alpha - B;
	Matrix E(n, n);
	for (int i = 0; i < n; i++)
		E[i][i] = 1;
	Matrix tmp = E - B;
	Matrix NAlpha = tmp*C;
	Vector Nbeta = tmp*beta;
	return iterative(NAlpha, Nbeta, eps);
}

int main() {
	std::string path = "matrix.txt";
	Matrix A;
	A.readMatrix(path);
	A.show();
	int n = A.size();
	Vector b;
	b.readVector("vector.txt");
	b.show();
	cout << endl;
	Matrix Alpha(n, n);
	Vector beta(n);
	toEquivalentForm(A, b, Alpha, beta);
	Alpha.show();
	beta.show();
	double eps;
	cout << "Enter epsilon: ";
	cin >> eps;
	Vector i_x = iterative(Alpha, beta, eps);
	Vector s_x = seidel(Alpha, beta, eps);

	i_x.show();
	s_x.show();
}
