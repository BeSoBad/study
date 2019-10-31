#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <string>

using namespace std;

vector <vector<double>> read_points(string path) {
	vector <vector <double>> res;
	ifstream f(path);
	if (!(f.is_open()))
		cout << "file not found" << endl;
	int count;
	double coef;
	f >> count;
	int step = 0;
	while (f >> coef) {
		res.push_back({coef});
		for (int i = 0; i < count - 1; i++) {
			f >> coef;
			res[step].push_back(coef);
		}
		step++;
	}
	f.close();
	return res;
}

double f(double x) {
	return atan(x);
}

double w(vector <double>& points, double x, int i) {
	double res = 1;
	for (int j = 0; j < points.size(); j++)
		if (j != i)
			res *= (x - points[j]);
	return res;
}

vector <double> get_coefs(vector <double> x, vector <double> y) {
	int cnt = x.size();
	vector <double> coefs(cnt);
	for (int i = 0; i < cnt; i++)
		coefs[i] = y[i];
	for (int i = 1; i < cnt; i++)
		for (int j = cnt - 1; j > i - 1; j--)
			coefs[j] = (coefs[j] - coefs[j - 1]) / (x[j] - x[j - i]);
	return coefs;
}

double lagrange(vector <double> points, double x) {
	double res = 0;
	for (int i = 0; i < points.size(); i++) {
		double f_w = f(points[i]) / w(points, points[i], i);
		res += f_w * w(points, x, i);
	}
	return res;
}

double newton(vector <double> points, double x) {
	vector <double> y(points.size());
	for (int i = 0; i < points.size(); i++)
		y[i] = f(points[i]);
	vector <double> coefs = get_coefs(points, y);
	double res = coefs[coefs.size() - 1];
	for (int i = coefs.size() - 1; i >= 0; i--)
		res = res * (x - points[i]) + coefs[i];
	return res;
}

int main() {
	vector <vector <double>> points = read_points("points.txt.txt");
	double x;
	cin >> x;
	vector <double> lagrange_res, newton_res;
	for (int i = 0; i < points.size(); i++) {
		double value = f(x);
		double lagrange_value = lagrange(points[i], x);
		double error = abs(value - lagrange_value);
		cout << "value - " << value << endl;
		cout << "lagrange value - " << lagrange_value << std::endl;
		cout << "error - " << error << endl;
		lagrange_res.push_back(lagrange_value);

		double newton_value = newton(points[i], x);
		error = abs(value - newton_value);
		cout << "newton value - " << newton_value << std::endl;
		cout << "error - " << error << endl << endl;
		newton_res.push_back(newton_value);
	}
}