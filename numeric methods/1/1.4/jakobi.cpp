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
	double norm() {
		double max = 0;
		for (int i = 0; i < n; i++) {
			double s = 0.0;
			for (int j = 0; j < n; j++) {
				s += abs(x[i][j]);
			}
			if (s > max)
				max = s;
		}
		return max;
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
	double norm() {
		double max = 0;
		for (int i = 0; i < n; i++) {
			double s = 0.0;
			s += abs(x[i]);
			if (s > max)
				max = s;
		}
		return max;
	}
};

Vector operator*(Matrix& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			C[i] += A[i][j] * B[j];
	return C;
}

Matrix operator*(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			for(int k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];
	return C;
}

Vector operator+(Vector& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		C[i] = A[i] + B[i];
	return C;
}

Vector operator-(Vector& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		C[i] = A[i] - B[i];
	return C;
}

Matrix operator-(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] - B[i][j];
	return C;
}

Matrix operator+(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] + B[i][j];
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

int main() {
    return 0;
}