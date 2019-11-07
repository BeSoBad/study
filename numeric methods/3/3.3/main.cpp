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

using namespace std;

const int X_COORD = 50;// X - размерность ] должны
const int Y_COORD = 50;// Y - размерность ] быть равными
const float ITERATIONS = 0.00005;// прорисовка графика (чем меньше тем лучше)

int x_off = X_COORD / 2;// начало
int y_off = Y_COORD / 2;// оси координат

//исходная функция
#define expr x
#define expr2 x*x


void drawgrid(float SERIF_OFFSET, float SERIF_DISTANCE) {
	glBegin(GL_LINES);
	//задаем цвета
	glColor3f(0.0, 0.0, 0.0);

	//рисуем координатные оси
	//горизонталь
	glVertex2f(0.0, Y_COORD / 2);
	glVertex2f(X_COORD, Y_COORD / 2);
	//засечки по горизонтали
	int p = X_COORD / 2;
	for (int i = X_COORD / 2; i < X_COORD; i += SERIF_DISTANCE, p -= SERIF_DISTANCE) {
		glVertex2f(i, Y_COORD / 2);
		glVertex2f(i, Y_COORD / 2 + SERIF_OFFSET);

		glVertex2f(p, Y_COORD / 2);
		glVertex2f(p, Y_COORD / 2 + SERIF_OFFSET);
	}
	//вертикаль
	int t = Y_COORD / 2;
	glVertex2f(X_COORD / 2, Y_COORD);
	glVertex2f(X_COORD / 2, 0.0);
	//засечки по вертикали
	for (int i = Y_COORD / 2; i < Y_COORD; i += SERIF_DISTANCE, t -= SERIF_DISTANCE) {
		glVertex2f(X_COORD / 2, i);
		glVertex2f(Y_COORD / 2 + SERIF_OFFSET, i);

		glVertex2f(X_COORD / 2, t);
		glVertex2f(Y_COORD / 2 + SERIF_OFFSET, t);
	}
	glEnd();
}

void drawfunc(vector <double>& x, vector <double>& y, double color) {
	//рисуем график
	glBegin(GL_LINES);
	float j = 0;
	glColor3f(color, 0.0, 0.0);
	//for (float x = -X_COORD * 2; x < X_COORD * 2; x += 0.5) {
	//	//перерасчитываем координаты
	//	j = expr;
	//	glVertex2d(x_off + x, y_off + j);//не убирать x и y!! это оффсет по осям!
	//}
	for (int i = 0; i < x.size() - 1; i++) {
		glVertex2f(x_off + x[i] * 5, y_off + y[i] * 5);
		glVertex2f(x_off + x[i + 1] * 5, y_off + y[i + 1] * 5);
	}
	glEnd();
}


void funcinfo(int val1, int val2) {
	//информация о графике
	for (float x = val1; x <= val2; x++) {
		float j = expr;
		cout << x << " : " << j << endl;
	}
}

vector <double> points_g, F_g;
double color_g;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "Osnovnie toshki po vashemu grafiku: \n";
	//vector <double>& points, vector <double>& F, double color

	drawgrid(0.3, 5);
	drawfunc(points_g, F_g, color_g);
	glutSwapBuffers();

	glFlush();
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

double f(double a, double b, double c, double d, double x) {
	return a + b * x + c * x * x + d * x * x * x;
}

vector <double> get_a(vector <double>& f) {
	vector <double> a(1);
	for (int i = 0; i < f.size() - 1; i++)
		a.push_back(f[i]);
	return a;
}

vector <double> get_b(vector <double>& f, vector <double>& h, vector <double>& c) {
	vector <double> b(1);
	int n = f.size() - 1;
	for (int i = 1; i < n; i++) {
		double p1 = (f[i] - f[i - 1]);
		double p2 = h[i];
		double p3 = double(1) / 3;
		double p4 = h[i] * (c[i + 1] + 2 * c[i]);
		b.push_back(p1 / p2 - p3 * p4);
		//b.push_back((f[i] - f[i - 1]) / h[i] - double(1) / 3 * h[i] * (c[i + 1] + 2 * c[i]));
	}
	b.push_back((f[n] - f[n - 1]) / h[n] - double(2) / 3 * h[n] * c[n]);
	return b;
}

vector <double> run_method(int n, vector <double>& a, vector <double>& c, vector <double>& b, vector <double>& f) {
	vector <double> x(n);
	double m;
	for (int i = 1; i < n; i++) {
		m = a[i] / c[i - 1];
		c[i] = c[i] - m * b[i - 1];
		f[i] = f[i] - m * f[i - 1];
	}

	x[n - 1] = f[n - 1] / c[n - 1];

	for (int i = n - 2; i >= 0; i--)
		x[i] = (f[i] - b[i] * x[i + 1]) / c[i];
	return x;
}

vector <double> get_c(vector <double>& f, vector <double>& h) {
	int n = f.size();
	vector <double> a(1), b, c, d;
	for (int i = 3; i < n; i++)
		a.push_back(h[i - 1]);
	for (int i = 2; i < n; i++)
		b.push_back(2 * (h[i - 1] + h[i]));
	for (int i = 2; i < n - 1; i++)
		c.push_back(h[i]);
	c.push_back(0);
	for (int i = 2; i < n; i++)
		d.push_back(3 * ((f[i] - f[i - 1]) / h[i] - ((f[i - 1] - f[i - 2]) / h[i - 1])));
	vector <double> x = run_method(a.size(), a, c, b, d);
	vector <double> res(2);
	for (int i = 0; i < x.size(); i++)
		res.push_back(x[i]);
	return res;
}

vector <double> get_d(vector <double>& h, vector <double>& c) {
	vector <double> d(1);
	int n = c.size() - 1;
	for (int i = 1; i < n; i++)
		d.push_back((c[i + 1] - c[i]) / (3 * h[i]));
	d.push_back(-c[n] / (3 * h[n]));
	return d;
}

int find_interval(vector <double>& points, double x) {
	for (int i = 0; i < points.size(); i++)
		if (points[i] <= x && points[i + 1] >= x)
			return i;
}

tuple <double, vector <double>, vector <double>, vector <double>, vector <double>> 
spline_interpolation(vector <double>& points, vector <double>& values, double x) {
	vector <double> h(1);
	for (int i = 1; i < points.size(); i++)
		h.push_back(points[i] - points[i - 1]);
	vector <double> c = get_c(values, h);
	vector <double> a = get_a(values);
	vector <double> b = get_b(values, h, c);
	vector <double> d = get_d(h, c);
	int i = find_interval(points, x);
	double res = f(a[i + 1], b[i + 1], c[i + 1], d[i + 1], x - points[i]);
	return make_tuple(res, a, b, c, d);
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


int main(int argc, char** argv) {
	
	auto data = read_data("input.txt", 5);
	vector <double> points = get<0>(data), values = get<1>(data);
	double x = 3;
//	cin >> x;

	auto res = spline_interpolation(points, values, x);

	double value = get<0>(res);
	vector <double> a = get<1>(res);
	vector <double> b = get<2>(res);
	vector <double> c = get<3>(res);
	vector <double> d = get<4>(res);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("GLUT_TESTING_APP");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//////пространство координат
	glOrtho(0.0, X_COORD, 0.0, Y_COORD, -1.0, 1.0);
  
	for (int i = 0; i < points.size() - 1; i++) {
		vector <double> x1 = linspace(points[i], points[i + 1], 10);
		vector <double> y1;
		for (int j = 0; j < x1.size(); j++) {
			double r = f(a[i + 1], b[i + 1], c[i + 1], d[i + 1], x1[j] - points[i]);
			y1.push_back(r);
		}
		for (int j = 0; j < x1.size(); j++) {
			points_g.push_back(x1[j]);
			F_g.push_back(y1[j]);
		}
	}

	color_g = 1.0;

	glutDisplayFunc(display);
	glutMainLoop();
}