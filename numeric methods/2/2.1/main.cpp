#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>

//методы простой итерации и Ньютона решения нелинейных уравнений

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
	auto f = freopen("log.txt", "w", stdout);
	double eps = 0.001, a = 0, b = 0.5;
	auto res = method_newton(eps, a, b);
	printf("interval: (%f, %f)\n", a, b);
	printf("newton method:\n");
	printf("x = %f, count iteraions = %d\n", res.first, res.second);
	res = method_iterations(eps, a, b);
	printf("iteration method:\n");
	printf("x = %f, count iteraions = %d", res.first, res.second);
	return 0;
}