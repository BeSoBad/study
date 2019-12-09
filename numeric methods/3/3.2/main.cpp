#include <windows.h>
#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <tuple>
#include "include/matrix.cpp"
#include "include/vector.cpp"
#include "include/LU.cpp"

////Кубический сплайн для функции, 
//заданной в узлах интерполяции, предполагая, 
//что сплайн имеет нулевую кривизну
////x0=-0.5
vector <double> points_g, F_g, inp_points, inp_values;
double color_g;


template<typename T>
std::ostream& operator<<(std::ostream& s, std::vector<T> t) {
	s << "[";
	for (std::size_t i = 0; i < t.size(); i++) {
		s << t[i] << (i == t.size() - 1 ? "" : ", ");
	}
	return s << "]" << std::endl;
}



template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in) {
	std::vector<double> linspaced;
	double start = static_cast<double>(start_in);
	double end = static_cast<double>(end_in);
	double num = static_cast<double>(num_in);
	if (num == 0) { return linspaced; }
	if (num == 1)
	{
		linspaced.push_back(start);
		return linspaced;
	}
	double delta = (end - start) / (num - 1);
	for (int i = 0; i < num - 1; ++i)
	{
		linspaced.push_back(start + delta * i);
	}
	linspaced.push_back(end);
	return linspaced;
}

void write_dots(string path) {
	ofstream f;
	f.open(path);
	f << points_g.size() << endl << "g" << endl;
	f << "cubic spline\nplot" << endl;
	for (int i = 0; i < points_g.size(); i++) {
		f << points_g[i] << " " << F_g[i] << endl;
	}
	f << inp_points.size() << endl << "r" << endl;
	f << "input dots\nscatter" << endl;
	for (int i = 0; i < inp_points.size(); i++) {
		f << inp_points[i] << " " << inp_values[i] << endl;
	}
	f.close();
}

#include <cstdlib>
#include <cmath>
#include <limits>

class cubic_spline
{
private:
	struct spline_tuple
	{
		double a, b, c, d, x;
	};

	spline_tuple* splines;
	std::size_t n; 

	void free_mem(); 

public:
	cubic_spline();
	~cubic_spline();
	void build_spline(vector <double> x, vector <double> y, std::size_t n);
	double f(double x) const;
};

cubic_spline::cubic_spline() : splines(NULL)
{

}

cubic_spline::~cubic_spline()
{
	free_mem();
}

void cubic_spline::build_spline(vector <double> x, vector <double> y, std::size_t n)
{
	free_mem();

	this->n = n;

	splines = new spline_tuple[n];
	for (std::size_t i = 0; i < n; ++i)
	{
		splines[i].x = x[i];
		splines[i].a = y[i];
	}
	splines[0].c = 0.;

	double* alpha = new double[n - 1];
	double* beta = new double[n - 1];
	double A, B, C, F, h_i, h_i1, z;
	alpha[0] = beta[0] = 0.;
	for (std::size_t i = 1; i < n - 1; ++i)
	{
		h_i = x[i] - x[i - 1], h_i1 = x[i + 1] - x[i];
		A = h_i;
		C = 2. * (h_i + h_i1);
		B = h_i1;
		F = 6. * ((y[i + 1] - y[i]) / h_i1 - (y[i] - y[i - 1]) / h_i);
		z = (A * alpha[i - 1] + C);
		alpha[i] = -B / z;
		beta[i] = (F - A * beta[i - 1]) / z;
	}

	splines[n - 1].c = (F - A * beta[n - 2]) / (C + A * alpha[n - 2]);

	for (std::size_t i = n - 2; i > 0; --i)
		splines[i].c = alpha[i] * splines[i + 1].c + beta[i];

	delete[] beta;
	delete[] alpha;

	for (std::size_t i = n - 1; i > 0; --i)
	{
		double h_i = x[i] - x[i - 1];
		splines[i].d = (splines[i].c - splines[i - 1].c) / h_i;
		splines[i].b = h_i * (2. * splines[i].c + splines[i - 1].c) / 6. + (y[i] - y[i - 1]) / h_i;
	}
}

double cubic_spline::f(double x) const
{
	if (!splines)
		return std::numeric_limits<double>::quiet_NaN();

	spline_tuple* s;
	if (x <= splines[0].x) 
		s = splines + 1;
	else if (x >= splines[n - 1].x) 
		s = splines + n - 1;
	else
	{
		std::size_t i = 0, j = n - 1;
		while (i + 1 < j)
		{
			std::size_t k = i + (j - i) / 2;
			if (x <= splines[k].x)
				j = k;
			else
				i = k;
		}
		s = splines + j;
	}

	double dx = (x - s->x);
	return s->a + (s->b + (s->c / 2. + s->d * dx / 6.) * dx) * dx; 
}

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

void cubic_spline::free_mem()
{
	delete[] splines;
	splines = NULL;
}

int main(int argc, char** argv) {
	auto ff = freopen("log.txt", "w", stdout);
	auto data = read_data("input.txt", 5);
	vector <double> points = get<0>(data), values = get<1>(data);
	double x = -0.5;
	//cin >> x;
  
	cubic_spline c_p;

	c_p.build_spline(points, values, points.size());

	double res = c_p.f(0.5);
	cout << c_p.f(0.5) << endl;

	for (int i = 0; i < points.size() - 1; i++) {
		vector <double> x1 = linspace(points[i], points[i + 1], 10);
		vector <double> y1;
		for (int j = 0; j < x1.size(); j++) {
			double r = c_p.f(x1[j]);
			y1.push_back(r);
		}
		for (int j = 0; j < x1.size(); j++) {
			points_g.push_back(x1[j]);
			F_g.push_back(y1[j]);
		}
	}

	inp_points = points;
	inp_values = values;
	color_g = 1.0;

	write_dots("dots.txt");

	system("python plot.py");
}