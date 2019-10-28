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
    Matrix H(size, size, 0), E(size, size, 1), tmp_m = v*v;
    Matrix tmp_m2 = tmp_m*(2*(1/get_mult_num(v, v)));
    H = E - tmp_m;
    return H;
}

Vector get_column(Matrix &A, int i) {
    Vector col(0);
    for (int j = 0; j < A.size(); j++) {
        col.push(A[j][i]);
    }
    return col;
}

int get_QR(Matrix &A, Matrix& Q1, Matrix& R) {
    int size = A.size();
    Matrix Q(size, size, 1);
    Matrix A_i = A;
    for (int i = 0; i < size - 1; i++) {
        Vector col = get_column(A, i);
        Matrix H = householder(col, size, i);
        Matrix tmp1 = Q*H, tmp2 = A_i*H;
        Q = tmp1;
        A_i = tmp2;
    }
    Q1 = Q;
    R = A_i;
}



std::pair<int, int> get_roots(Matrix& A, int i) {
    int size = A.size();
    double a11 = A[i][i];
    double a12 = (i + 1 < size) ? A[i][i + 1]:0;
    double a21 = (i + 1 < size) ? A[i + 1][i]:0;
    double a22 = (i + 1 < size) ? A[i + 1][i + 1]:0;
    double c = 1, b = -a11 - a22, a = a11 * a22 - a12 * a21;
    double x1, x2;
    if ((b*b - 4*a*c) >= 0) {
        x1 = (-b + sqrt(b*b - 4*a*c))/(2 * a);
        x2 = (-b - sqrt(b*b - 4*a*c))/(2 * a);
    }
    return std::make_pair(x1, x2);
}

bool finish_iter_for_complex(Matrix& A, double eps, int i) {
    Matrix Q, R;
    get_QR(A, Q, R);
    Matrix A_n = R*Q;
    std::pair<int, int> l1 = get_roots(A, i), l2 = get_roots(A_n, i);
    if (std::abs(l1.first - l1.first) <= eps && std::abs(l1.second - l2.second) <= eps) {
        return true;
    }
    else
        return false;
}

std::tuple<double, bool, Matrix> get_eigenvalues(Matrix& A, double eps, int i) {
	Matrix A_i = A;
    std::tuple <double, bool, Matrix> res;
	while (1) {
		Matrix Q, R;
		get_QR(A_i, Q, R);
        A_i = R*Q;
        if (finish_iter_for_complex(A_i, eps, i)) {
            res = std::make_tuple(A_i[i][i], true, A_i);
            break;
        }
	}
    return res;
}
    
std::tuple<Vector, int> QR(Matrix& A, double eps) {
	Vector res;
	int i = 0, size = A.size();
	Matrix A_i = A;
	while (i < size) {
		std::tuple<double, bool, Matrix> eval = get_eigenvalues(A_i, eps, i);
        if (std::get<1>(eval)) {
            res.push(std::get<0>(eval));
            i += 2;
        }
	}
    return make_tuple(res, i);
}

int main() {
	std::string path = "/home/bsb/github/study/numeric methods/1/1.5/build/matrix.txt";
	Matrix A;
	A.readMatrix(path);
	A.show();
	int n = A.size();
	Vector b;
	b.readVector("/home/bsb/github/study/numeric methods/1/1.5/build/vector.txt");
	b.show();
	
	double eps = 0.01;
	//std::cin >> eps;
	
	std::tuple<Vector, int> res = QR(A, eps);
    
    Vector eval = std::get<0>(res);

    eval.show();
}
