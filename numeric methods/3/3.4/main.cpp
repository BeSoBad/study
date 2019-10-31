#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <string>
#include <tuple>

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

int find_interval(vector <double> points, double x) {
	for (int i = 0; i < points.size() - 1; i++)
		if (points[i] <= x && points[i + 1] >= x)
			return i;
}

double first_derivative(vector <double> x, vector <double> y, double x0) {
	int i = find_interval(x, x0);
	double addend1 = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
	double addend2 = ((y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1]) - addend1) / (x[i + 2] - x[i]) * (2 * x0 - x[i] - x[i + 1]);
	return addend1 + addend2;
}

double second_derivative(vector <double> x, vector <double> y, double x0) {
	int i = find_interval(x, x0);
	double num1 = (y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1]);
	double num2 = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
	return 2 * (num1 - num2) / (x[i + 2] - x[i]);
}



int main() {
	auto data = read_data("input.txt", 5);
	vector <double> points = get<0>(data), values = get<1>(data);
	double x;
	cin >> x;
	double first_d = first_derivative(points, values, x);
	double second_d = second_derivative(points, values, x);
	cout << first_d << " " << second_d << endl;
}