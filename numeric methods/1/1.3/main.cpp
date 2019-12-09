#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "vector.h"
#include "operations.h"


using namespace std;

//----From LU.cpp---//

Matrix get_P(Matrix& A) {
	Matrix P(A.size(), A.size());
	for (int i = 0; i < P.size(); i++)
		P[i][i] = 1;
	int max_row = 0;
	for (int i = 0; i < P.size(); i++) {
		double max_elem = -1e9;
		int max_ind = -1;
		for (int j = i; j < P.size(); j++) {
			if (abs(A[j][i]) >= max_elem) {
				max_elem = A[j][i];
				max_ind = j;
			}
		}
		swap(P[i], P[max_ind]);
	}
	return P;
}

int find_int(vector <double>& v, int x) {
	int res = 0;
	for (int i = 0; i < v.size(); i++)
		if (v[i] == x)
			res = i;
	return res;
}

Vector get_new_B(Vector b, Matrix& P) {
	vector <double>	perm;
	for (int i = 0; i < P.size(); i++)
		perm.push_back(find_int(P[i], 1));
	Vector new_b(b.size());
	for (int i = 0; i < b.size(); i++)
		new_b[i] = b[perm[i]];
	return new_b;
}

Matrix LU(Matrix A, Matrix& LU, int n) {
	Matrix P = get_P(A);
	Matrix tmp = P * A;
	LU = tmp;
	for (int i = 0; i < A.size(); i++) {
		for (int j = i + 1; j < A.size(); j++) {
			LU[j][i] /= LU[i][i];
			for (int k = i + 1; k < A.size(); k++)
				LU[j][k] -= LU[j][i] * LU[i][k];
		}
	}
	return P;
}

Vector solve(Matrix& LU, Vector& b) {
	int n = b.size();
	Vector z(n);
	z[0] = b[0];
	for (int i = 1; i < n; i++) {
		double sum = 0;
		for (int j = 0; j < i; j++) {
			sum += LU[i][j] * z[j];
		}
		z[i] = b[i] - sum;
	}
	Vector x(n);
	x[n - 1] = z[n - 1] / LU[n - 1][n - 1];
	for (int i = n - 1; i >= 0; i--) {
		x[i] = z[i];
		for (int j = i + 1; j < n; j++) {
			x[i] -= LU[i][j] * x[j];
		}
		x[i] /= LU[i][i];
	}
	return x;
}

double determinant(Matrix& L, Matrix& U) {
	double detL = 1, detU = 1;
	for (int i = 0; i < L.size(); i++) {
		detL *= L[i][i];
		detU *= U[i][i];
	}
	return detL * detU;
}

Matrix inverse(Matrix& LU) {
	int n = LU.size();
	Vector e(n);
	Matrix IAT(n, n);
	for (int i = 0; i < n; i++) {
		e[i] = 1;
		Vector v = solve(LU, e);
		IAT[i] = v.x;
		e[i] = 0;
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

Matrix tril(Matrix& A) {
	Matrix res;
	res = A;
	int n = A.size();
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			res[i][j] = 0;
	return res;
}

Vector seidel(Matrix& Alpha, Vector& beta, double eps) {
	int n = Alpha.size();
	Matrix IAlpha = tril(Alpha);
	Matrix B = IAlpha;
	Matrix C = Alpha - B;
	Matrix E(n, n);
	for (int i = 0; i < n; i++)
		E[i][i] = 1;
	Matrix tmp = E - B, tmp_LU(n, n);
	LU(tmp, tmp_LU, n);
	Matrix tmp_inv = inverse(tmp_LU);
	Matrix NAlpha = tmp_inv*C;
	Vector Nbeta = tmp_inv*beta;
	return iterative(NAlpha, Nbeta, eps);
}

int main() {
	auto f = freopen("log.txt", "w", stdout);
	std::string path = "matrix.txt";
	Matrix A;
	A.readMatrix(path);
	cout << "input matrix:\n";
	A.show();
	int n = A.size();
	Vector b;
	b.readVector("vector.txt");
	cout << "input vector:\n";
	b.show();
	cout << endl;
	Matrix Alpha(n, n);
	Vector beta(n);
	toEquivalentForm(A, b, Alpha, beta);
	cout << "Alpha:\n";
	Alpha.show();
	cout << "beta:\n";
	beta.show();
	double eps;
	cout << "epsilon: ";
	cin >> eps;
	cout << eps << endl;
	Vector i_x = iterative(Alpha, beta, eps);
	Vector s_x = seidel(Alpha, beta, eps);

	cout << "iterative answer:\n";
	i_x.show();
	cout << "seidel answer:\n";
	s_x.show();
}
