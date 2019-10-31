#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>

const double E = 2.71828182845904523536;


double f(double x) {
	return std::pow(E, x) - x * x * x + 3 * x * x - 2 * x - 3;
}

double df(double x) {
	return std::pow(E, x) - 3 * x * x + 6 * x - 2;
}

double phi(double x) {
	return std::log(x * x * x - 3 * x * x + 2 * x + 3);
}

double dphi(double x) {
	return (3 * x * x - 6 * x + 2) / (x * x * x - 3 * x * x + 2 * x + 3);
}

std::pair<double, int> method_iterations(double eps, double a, double b) {
	double x_prev = b, x;
	int iter_cnt = 0;
	double q = std::fmax(std::abs(dphi(a)), std::abs(dphi(b)));
	while (1) {
		iter_cnt++;
		x = phi(x_prev);
		double finish = std::abs(x - x_prev) * q / (1 - q);
		if (finish <= eps)
			break;
		x_prev = x;
	}
	return std::make_pair(x, iter_cnt);
}

std::pair<double, int> method_newton(double eps, double a, double b) {
	double x_prev = b, x;
	int iter_cnt = 0;
	while (1) {
		iter_cnt++;
		x = x_prev - f(x_prev) / df(x_prev);
		double finish = abs(f(x) - f(x_prev));
		if (finish <= eps)
			break;
		x_prev = x;
	}
	return std::make_pair(x, iter_cnt);
}



int main() {
	double eps = 0.01;
	auto res = method_newton(eps, 0, 0.5);
	std::cout << res.first << " " << res.second << std::endl;
	res = method_iterations(eps, 0, 0.5);
	std::cout << res.first << " " << res.second << std::endl;
	return 0;
}