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
	Matrix A = readMatrix("matrix.txt");
	A.show();
	int n = A.size();
	Vector b = readVector("vector.txt");
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
