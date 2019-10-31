#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <string>
#include <tuple>

#define CalcResult tuple <double, double, double, double>

using namespace std;

tuple <vector<double>, vector<double>> read_data(string path, int count) {
	ifstream f(path);
	if (!(f.is_open()))
		cout << "file not found" << endl;
	vector <double> points(count), values(count);
	for (int i = 0; i < count; i++)
		f >> points[i];
	for (int i = 0; i < count; i++)
		f >> values[i];
	f.close();
	return make_tuple(points, values);
}

double f(double x) {
	return 1 / (x * x * x + 64);
}

vector <double> get_values(vector <double> points) {
	vector <double> res;
	for (auto x: points)
		res.push_back(f(x));
	return res;
}

vector <double> get_points(double x0, double x, double step) {
	vector <double> points;
	for (int i = x0; i < x + step; i += step)
		points.push_back(i);
	return points;
}

double rectangle_method(vector <double> x, double h) {
	double sum = 0;
	for (int i = 0; i < x.size() - 1; i++)
		sum += f((x[i] + x[i + 1]) / 2);
	return h * sum;
}

double trapeze_method(vector <double> y, double h) {
	int n = y.size() - 1;
	double sum = 0;
	for (int i = 1; i < n - 1; i++)
		sum += y[i];
	return h * (y[0] / 2 + sum + y[n] / 2);
}

double simpson_method(vector <double> y, double h) {
	int n = y.size() - 1;
	double sum1 = 0, sum2 = 0;
	for (int i = 1; i < n; i += 2)
		sum1 += 4 * y[i];
	for (int i = 2; i < n - 1; i += 2)
		sum2 += 2 * y[i];
	return h / 3 * (y[0] + sum1 + sum2 + y[n]);
}

tuple <vector<double>, vector<double>, vector<double>> runge_method(vector <CalcResult> res) {
	double value = 1.8574186872187473;
	double k = get<0>(res[0]) / get<0>(res[1]);
	vector <double> err_rec = { abs(get<1>(res[0]) - get<1>(res[1])) / (k * k - 1), abs(get<1>(res[0]) - value) / (k * k - 1) };
	vector <double> err_trap = { abs(get<2>(res[0]) - get<2>(res[1])) / (k * k - 1), abs(get<2>(res[0]) - value) / (k * k - 1) };
	vector <double> err_sim = { abs(get<3>(res[0]) - get<3>(res[1])) / (k * k * k * k - 1), abs(get<3>(res[0]) - value) / (k * k * k * k - 1) };
	return make_tuple(err_rec, err_trap, err_sim);
}

int main() {
	auto data = read_data("input.txt", 5);
	vector <double> points = get<0>(data), values = get<1>(data);
	double x0, x;
	double steps[2];
	cin >> x0 >> x >> steps[0] >> steps[1];
	for (auto h: steps) {
		vector <double> points = get_points(x0, x, h);
		vector <double> values = get_values(points);
		double rec = rectangle_method(points, h);
		double trap = trapeze_method(values, h);
		double sim = simpson_method(values, h);
		cout << rec << " " << trap << " " << sim << endl;	
	}
}