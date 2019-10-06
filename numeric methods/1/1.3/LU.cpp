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

int main() {
	Matrix A = readMatrix("matrix.txt");
	A.show();
	int n = A.size();
	Matrix L(n, n);
	Matrix U(n, n);
	LU(A, L, U, A.n);
	L.show();
	U.show();
	Matrix AA = L*U;
	AA.show();
	Vector b = readVector("vector.txt");
	b.show();
	Vector x = solve(L, U, b);
	x.show();
	cout << "detA: " << determinant(L, U) << endl;
	Matrix IAT = inverse(L, U);
	cout << "inversing:\n";
	IAT.show();
}
