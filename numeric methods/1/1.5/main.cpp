#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <utility>
#include <tuple>
#include "matrix.h"
#include "vector.h"
#include "operations.h"

using namespace std;

int sign(double a) {
	if (a == 0)
		return 0;
	else if (a > 0)
		return 1;
	else
		return -1;
}

double get_mult_num(Vector& a, Vector& b) {
    double res = 0;
    for (int i = 0; i < a.size(); i++) {
        res += a[i]*b[i];
    }
    return res;
}

Matrix householder(Vector& col, int size, int i) {
    Vector v(size);
    Vector tmp(size);
    for (int j = i; j < size; j++)
        tmp[j] = col[j];
    v[i] = col[i] + sign(col[i])*tmp.norm();
	for (int j = i + 1; j < size; j++)
		v[j] = col[j];
    Matrix H(size, size, 0), E(size, size, 1), tmp_m = v*v;
    Matrix tmp_m2 = tmp_m*(2/get_mult_num(v, v));
    H = E - tmp_m2;
    return H;
}

Vector get_column(Matrix& A, int i) {
	Vector col(0);
	for (int j = 0; j < A.size(); j++) {
		col.push(A[j][i]);
	}
	return col;
}

Vector get_sub_diagonal(Matrix& A, int indent, int i_column) {
	Vector col(0);
	for (int j = indent; j < A.size(); j++) {
		col.push(A[j][i_column]);
	}
	if (col.size() == 0)
		col.push(0);
	return col;
}

int get_QR(Matrix &A, Matrix& Q1, Matrix& R) {
    int size = A.size();
    Matrix Q(size, size, 1);
    Matrix A_i = A;
    for (int i = 0; i < size - 1; i++) {
        Vector col = get_column(A_i, i);
        Matrix H = householder(col, size, i);
        Matrix tmp1 = Q*H, tmp2 = H*A_i;
        Q = tmp1;
        A_i = tmp2;
    }
    Q1 = Q;
    R = A_i;
	return 0;
}

double not_complex = 1e6 + 23;

struct root {
	double real, image;
	bool complex;
	root() {
		real = 0;
		image = not_complex;
		complex = false;
	}
};

struct roots {
	root first, second;
	bool complex_pair;
	roots() {
		complex_pair = false;
	}
	void make_complex() {
		complex_pair = true;
		second.complex = true;
	}
	void make_real() {
		complex_pair = false;
		first.complex = false;
		second.complex = false;
	}
};

roots get_roots(Matrix& A, int i) {
    int size = A.size();
    double a11 = A[i][i];
    double a12 = (i + 1 < size) ? A[i][i + 1]:0;
    double a21 = (i + 1 < size) ? A[i + 1][i]:0;
    double a22 = (i + 1 < size) ? A[i + 1][i + 1]:0;
    double a = 1, b = -a11 - a22, c = a11 * a22 - a12 * a21;
	roots rs;
	double D = b * b - 4 * a * c;
	root r1, r2;
    if (D >= 0) {
        r1.real = (-b + sqrt(D))/(2 * a);
		r2.real = (-b - sqrt(D))/(2 * a);
    }
	else {
		r1.real = -b / (2.0 * a);
		r1.image = sqrt(-D) / (2.0 * a);
		r2.real = -b / (2.0 * a);
		r2.image = -sqrt(-D) / (2.0 * a);
		rs.make_complex();
	}
	rs.first = r1;
	rs.second = r2;
	return rs;
}

bool finish_iter_for_complex(Matrix& A, double eps, int i) {
    Matrix Q, R;
    get_QR(A, Q, R);
    Matrix A_n = R*Q;
    roots l1 = get_roots(A, i), l2 = get_roots(A_n, i);
	if (!l1.complex_pair && !l2.complex_pair) {

	}
    if (abs(l1.first - l2.first) <= eps && abs(l1.second - l2.second) <= eps) {
        return true;
    }
    else
        return false;
}

tuple <pair <double, double>, bool, Matrix> get_eigenvalues(Matrix& A, double eps, int i) {
	Matrix A_i = A;
    tuple <pair <double, double>, bool, Matrix> res;
	int cnt = 0;
	while (1) {
		Matrix Q, R;
		get_QR(A_i, Q, R);
        A_i = R*Q;
		Vector subdiag1 = get_sub_diagonal(A_i, i + 1, 0);
		Vector subdiag2 = get_sub_diagonal(A_i, i + 2, 0);
		double norm1 = subdiag1.norm();
		double norm2 = subdiag2.norm();
		bool finish_iter = finish_iter_for_complex(A_i, eps, i);
		if (finish_iter)
			cout << "Ftrue" << endl;
		if (subdiag1.norm() <= eps) {
			auto a = make_pair(A_i[i][i], 0);
			res = make_tuple(a, false, A_i);
			break;
		}
		else if (subdiag2.norm() <= eps && finish_iter_for_complex(A_i, eps, i)) {
			auto a = get_roots(A_i, i);
            res = make_tuple(a, true, A_i);
            break;
        }
		cnt++;
	}
    return res;
}
    
tuple<vector <double>, int> QR(Matrix& A, double eps) {
	vector <double> res;
	int i = 0, size = A.size();
	Matrix A_i = A;
	while (i < size) {
		tuple<pair <double, double>, bool, Matrix> eval = get_eigenvalues(A_i, eps, i);
        if (get<1>(eval)) {
            res.push_back(get<0>(eval).first);
			res.push_back(get<0>(eval).second);
            i += 2;
        }
		else {
			res.push_back(get<0>(eval).first);
            i += 1;
		}
	}
    return make_tuple(res, i);
}

int main() {
	double eps = 0.01;
	cout << "Enter epsilon: ";
	cin >> eps;
	auto f = freopen("log.txt", "w", stdout);
	string path = "matrix.txt";
	Matrix A;
	A.readMatrix(path);
	cout << "input matrix:\n";
	A.show();
	int n = A.size();
	Vector b(A.size());
	b.readVector("vector.txt");
	cout << "input vector:\n";
	b.show();

	
	tuple<vector <double>, int> res = QR(A, eps);
    
	vector <double> eval = get<0>(res);
	cout << "eigenvalues:\n";
	for (int i = 0; i < eval.size(); i++) {
		cout << eval[i] << endl;
	}
    //eval.show();
}
