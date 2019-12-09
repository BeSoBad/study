#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "vector.h"
#include "operations.h"

using namespace std;

//метод прогонки 

Vector TMA(Matrix& A, Vector& d) {
	int n = A.size();
	vector <double> P, Q;
	Vector x(n);
	P.push_back(-A[0][1]/A[0][0]);
	Q.push_back(d[0]/A[0][0]);

	for (int i = 1; i < n; i++) {
		double P_i, Q_i;
		double a = A[i][i - 1], b = A[i][i], c;
		if (i == n - 1) {
			P_i = 0;
			c = 0;
		}
		else {
			c = A[i][i + 1];
			P_i = -c/(b + a*P[i - 1]);
		}
		Q_i = (d[i] - a*Q[i - 1])/(b + a*P[i - 1]);
		P.push_back(P_i);
		Q.push_back(Q_i);
	}

	x[n - 1] = Q[n - 1];
	for (int i = n - 2; i >= 0; i--)
		x[i] = P[i] * x[i + 1] + Q[i];
	return x;
}

int main() {
	auto f = freopen("log.txt", "w", stdout);
	Matrix A;
	std::string path = "matrix.txt";
	A.readMatrix(path);
	cout << "input matrix:\n";
	A.show();
	int n = A.size();
	Vector d;
	d.readVector("vector.txt");
	cout << "input vector:\n";
	d.show();
	Vector x = TMA(A, d);
	cout << "answer:\n";
	x.show();
}
