#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "vector.h"
#include "matrix.h"
#include "operations.h"

//LU разложение. 

using namespace std;


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

int main() {
	auto f = freopen("log.txt", "w", stdout);
	Matrix A;
	std::string path = "matrix.txt";
	A.readMatrix(path);
	A.show();
	int n = A.size();
	//Matrix L(n, n);
	//Matrix U(n, n);
	Matrix LU1(n, n);
	//Matrix P = LU(A, L, U, A.n);
	Matrix P = LU(A, LU1, A.n);
	//Matrix AA = L*U;
	cout << "LU:\n";
	LU1.show();
	Vector b;
	b.readVector("vector.txt");
	cout << "vector b:\n";
	b.show();
	Vector new_b = get_new_B(b, P);
	new_b.show();
	Vector x = solve(LU1, new_b);
	cout << "answer:\n";
	x.show();
	//cout << "detA: " << determinant(L, U) << endl;
	Matrix IAT = inverse(LU1);
	cout << "inversing:\n";
	IAT.show();
}
