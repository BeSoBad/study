#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

class Matrix {
public:
	vector <vector <double>> x;
	int n, m;
	vector <double>& operator[](int i) {
		return x[i];
	} 
	Matrix(int nn, int mm) {
		vector <vector <double>> tmp(nn, vector <double> (mm));
		x = tmp;
		n = nn;
		m = mm;
	}
	void show() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++)
				cout << x[i][j] << "\t";
			cout << "\n";
		}
		cout << endl;
	}
	int size() {
		return n;
	}		
	void transpose() {
		int t;
		Matrix tmp(n, n);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				tmp[i][j] = x[j][i];
			}
		}
		x = tmp.x;
	}
};

class Vector {
public:
	vector <double> x;
	int n;
	double& operator[](int i) {
		return x[i];
	}
	Vector(int nn) {
		vector <double> tmp(nn);
		x = tmp;
		n = nn;
	}
	void show() {
		for (int i = 0; i < n; i++) {
			cout << x[i] << "\t";
		}
		cout << endl;
	}
	int size() {
		return n;
	}
	void push(double t) {
		x.push_back(t);
		n++;
	}
};

Matrix operator*(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			for(int k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];
	return C;
}

Matrix readMatrix(string &&path) {
	ifstream in(path);

	if (in.is_open()) {
		int count = 0;
		int temp;
		vector <int> temp_v;
		while (!in.eof()) {
				in >> temp;
				temp_v.push_back(temp);
				count++;
		}
		int n = sqrt(count);
		Matrix x(n, n);
		int cnt = 0;
		for (int i = 0; i < n; i++) { 
			for (int j = 0; j < n; j++) {
				if (cnt >= count) {
					break;
				}
				in >> temp;
				x[i][j] = temp_v[cnt];
				cnt++;
			}
			if (cnt >= count) {
				break;
			}
		}
		return x;
	}
	else {
		cout << "Файл не найден.";
	}
}

Vector readVector(string &&path) {
	ifstream in(path);

	if (in.is_open()) {
		int count = 0;
		double temp;
		Vector x(0);
		while (!in.eof()) {
				in >> temp;
				x.push(temp);
		}
		x.n--;
		return x;
	}
	else {
		cout << "Файл не найден.";
	}
}

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
	Matrix A = readMatrix("matrix.txt");
	A.show();
	int n = A.size();
	Vector d = readVector("vector.txt");
	d.show();
	Vector x = TMA(A, d);
	x.show();
}
