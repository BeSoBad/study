#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <matrix.h>
#include <vector.h>
#include "include/LU.h"

using namespace std;

template<typename T>
std::ostream& operator<<(std::ostream& s, std::vector<T> t) {
	s << "[";
	for (std::size_t i = 0; i < t.size(); i++) {
		s << t[i] << (i == t.size() - 1 ? "" : ",");
	}
	return s << "]" << std::endl;
}

double f1(vector<double>& x) {
	return x[0] - cos(x[1]) - 2;
}

double f2(vector<double>& x) {
	return x[1] - sin(x[0]) - 2;
}

double phi1(vector<double>& x) {
	return 2 + cos(x[1]);
}

double phi2(vector<double>& x) {
	return 2 + sin(x[0]);
}

double df1(vector<double>& x, int base) {
	if (base == 0)
		return 1;
	else
		return sin(x[1]);
}

double df2(vector<double>& x, int base) {
	if (base == 0)
		return -cos(x[0]);
	else
		return 1;
}

double dphi1(vector<double>& x, int base) {
	if (base == 0)
		return cos(x[0]);
	else
		return 0;
}

double dphi2(vector<double>& x, int base) {
	if (base == 0)
		return 0;
	else
		return -sin(x[1]);
}

double get_q(vector<double>& x) {
	double max_phi1 = abs(dphi1(x, 0)) + abs(dphi1(x, 1));
	double max_phi2 = abs(dphi2(x, 0)) + abs(dphi2(x, 1));
	return fmax(max_phi1, max_phi2);
}

vector<vector<double>> A1(vector<double>& x) {
	vector<vector<double>> A(2, vector<double>(2));
	A[0][0] = f1(x);
	A[0][1] = df1(x, 1);
	A[1][0] = f2(x);
	A[1][1] = df2(x, 1);
	return A;
}

vector<vector<double>> A2(vector<double>& x) {
	vector<vector<double>> A(2, vector<double>(2));
	A[0][0] = df1(x, 0);
	A[0][1] = f1(x);
	A[1][0] = df2(x, 0);
	A[1][1] = f2(x);
	return A;
}


vector<vector<double>> jacobi(vector<double>& x) {
	vector<vector<double>> A(2, vector<double>(2));
	A[0][0] = df1(x, 0);
	A[0][1] = df1(x, 1);
	A[1][0] = df2(x, 0);
	A[1][1] = df2(x, 1);
	return A;
}

double det(vector<vector<double>>& A) {
	return A[0][0] * A[1][1] - A[0][1] * A[1][0];
}

Vector delta_x(vector<double>& x) {
	Matrix A(jacobi(x));
	Vector b(2);
	b[0] = -f1(x);
	b[1] = -f2(x);
	Matrix L(2, 2), U(2, 2);
	LU(A, L, U, 2);
	Vector delta = solve(L, U, b);
	return delta;
}

std::pair <vector<double>, int> method_iterations(double eps, vector<vector<double>> interval) {
	double iter_cnt = 0;
	vector <double> interval_x1 = interval[0], interval_x2 = interval[1];
	vector <double> x_prev(2), x;
	x_prev[0] = (interval_x1[1] - interval_x1[0]) / 2;
	x_prev[1] = (interval_x2[1] - interval_x2[0]) / 2;
	double q = get_q(x_prev);
	while (1) {
		iter_cnt++;
		x = { phi1(x_prev), phi2(x_prev) };
		double max_abs = abs(x[0] - x_prev[0]);
		for (int i = 0; i < x.size(); i++)
			if (abs(x[i] - x_prev[i]) > max_abs)
				max_abs = abs(x[i] - x_prev[i]);
		double finish = max_abs * q / (1 - q);
		cout << iter_cnt << ": " << x;
		cout << finish << "?" << eps << endl;
		if (finish <= eps)
			break;
		x_prev = x;
	}
	return make_pair(x, iter_cnt);
}

std::pair <vector<double>, int> method_newton(double eps, vector<vector<double>> interval, bool LU) {
	double iter_cnt = 0;
	vector <double> interval_x1 = interval[0], interval_x2 = interval[1];
	vector <double> x_prev(2), x;
	x_prev[0] = (interval_x1[1] - interval_x1[0]) / 2;
	x_prev[1] = (interval_x2[1] - interval_x2[0]) / 2;
	double q = get_q(x_prev);
	while (1) {
		iter_cnt++;
		if (LU) {
			Vector delta(2); 
			delta = delta_x(x_prev);
			x = { x_prev[0] + delta[0], x_prev[1] + delta[1] };
		}
		else {
			x[0] = x_prev[0] + det(A1(x_prev)) / det(jacobi(x_prev));
			x[1] = x_prev[1] + det(A2(x_prev)) / det(jacobi(x_prev));
		}
		double max_abs = abs(x[0] - x_prev[0]);
		for (int i = 0; i < x.size(); i++)
			if (abs(x[i] - x_prev[i]) > max_abs)
				max_abs = abs(x[i] - x_prev[i]);
		double finish = max_abs;
		cout << iter_cnt << ": " << x;
		cout << finish << "?" << eps << endl;
		if (finish <= eps)
			break;
		x_prev = x;
	}
	return make_pair(x, iter_cnt);
}

int main() {
	double eps = 0.01;
	vector <vector<double>> interval;
	interval.push_back({1, 2});
	interval.push_back({0.5, 1.5});
	auto res = method_newton(eps, interval, true);
	std::cout << res.first << " " << res.second << std::endl;
	res = method_iterations(eps, interval);
	std::cout << res.first << " " << res.second << std::endl;
	return 0;
}