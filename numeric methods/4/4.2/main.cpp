#include <windows.h>
#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <tuple>
#include <map>
#include "include/matrix.cpp"
#include "include/vector.cpp"
//#include <libconfig.h++>

#define storage map <string, map <string, vector <double>>>

using namespace std;

const double E = 2.71828182845904523536;

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

vector <storage> save_res;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "Osnovnie toshki po vashemu grafiku: \n";
	//vector <double>& points, vector <double>& F, double color

	drawgrid(0.3, 5);
	drawfunc(save_res[0]["Shooting"]["x"], save_res[0]["Shooting"]["y"], 1);
	drawfunc(save_res[0]["FD"]["x"], save_res[0]["FD"]["y"], 2);
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



double func(double x, double y, double y_der) {
	return (x * y_der - y) / (x * (x - 1));
}

double g(double x, double y, double z) {
	return z;
}

double p(double x) {
	return -x / (x * (x - 1));
}

double q(double x) {
	return 1 / (x * (x - 1));
}

double f(double x) {
	return 0;
}

double exact_func(double x) {
	return 1 + x + x * log(abs(x));
}

tuple<vector <double>, vector <double>, vector <double>> Runge_Kutta_method(double a, double b, double h, double y0, double y_der) {
	int n = (b - a) / h;
	vector <double> x, y, z;
	for (double i = a; i < b + h; i += h)
		x.push_back(i);
	z.push_back(y_der);
	y.push_back(y0);
	for (int i = 0; i < x.size() - 1; i++) {
		double K1 = h * g(x[i], y[i], z[i]);
		double L1 = h * func(x[i], y[i], z[i]);
		double K2 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1);
		double L2 = h * func(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1);
		double K3 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2);
		double L3 = h * func(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2);
		double K4 = h * g(x[i] + h, y[i] + K3, z[i] + L3);
		double L4 = h * func(x[i] + h, y[i] + K3, z[i] + L3);
		double delta_y = (K1 + 2 * K2 + 2 * K3 + K4) / 6;
		double delta_z = (L1 + 2 * L2 + 2 * L3 + L4) / 6;
		y.push_back(y[i] + delta_y);
		z.push_back(z[i] + delta_z);
	}
	return make_tuple(x, y, z);
}

double first_der(vector <double> x, vector <double> y, double x0) {
	int i = 0;
	while (i < x.size() - 1 && x[i + 1] < x0)
		i++;
	return (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
}

double get_n(double n_prev, double n, vector <vector <double>> ans_prev, vector <vector <double>> ans, double b, double delta, double gamma, double y1) {
	vector <double> x = ans_prev[0], y = ans_prev[1];
	double y_der = first_der(x, y, b);
	double phi_n_prev = delta * y[y.size() - 1] + gamma * y_der - y1;
	x = ans[0];
	y = ans[1];
	y_der = first_der(x, y, b);
	double phi_n = delta * y[y.size() - 1] + gamma * y_der - y1;
	return n - (n - n_prev) / (phi_n - phi_n_prev) * phi_n;
}

bool check_finish(vector <double> x, vector <double> y, double b, double delta, double gamma, double y1, double eps) {
	double y_der = first_der(x, y, b);
	return abs(delta * y[y.size() - 1] + gamma * y_der - y1) > eps;
}

tuple<vector <double>, vector <double>, vector <double>> shooting_method(double a, double b, double alpha, double beta, double delta, double gamma, double y0, double y1, double h, double eps) {
	double n_prev = 1.0, n = 0.8;
	double y_der = (y0 - alpha * n_prev) / beta;
	auto ans_prev = Runge_Kutta_method(a, b, h, n_prev, y_der);
	y_der = (y0 - alpha * n) / beta;
	auto ans = Runge_Kutta_method(a, b, h, n, y_der);

	while (check_finish(get<0>(ans), get<1>(ans), b, delta, gamma, y1, eps)) {
		vector <vector <double>> ans_prev_v = { get<0>(ans_prev),
									get<1>(ans_prev),
									get<2>(ans_prev)
		};
		vector <vector <double>> ans_v = { get<0>(ans),
									get<1>(ans),
									get<2>(ans)
		};
		n = get_n(n_prev, n, ans_prev_v, ans_v, b, delta, gamma, y1);
		n_prev = n;
		ans_prev = ans;
		y_der = (y0 - alpha * n) / beta;
		ans = Runge_Kutta_method(a, b, h, n, y_der);
	}
	return ans;
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

pair <vector <double>, vector <double>> finite_difference_method(double a, double b, double alpha, double beta, double delta, double gamma, double y0, double y1, double h) {
	int n = (int)((b - a) / h);
	vector <double> x, y, A, B, C, D;
	for (double i = a; i < b + h; i += h)
		x.push_back(i);
	n = x.size() - 1;
	A.push_back(0);
	for (int i = 0; i < n - 1; i++)
		A.push_back(1 - p(x[i]) * h / 2);
	A.push_back(-gamma);

	B.push_back(alpha * h - beta);
	for (int i = 0; i < n - 1; i++)
		B.push_back(q(x[i]) * h * h - 2);
	B.push_back(delta * h + gamma);

	C.push_back(beta);
	for (int i = 0; i < n - 1; i++)
		C.push_back(1 + p(x[i]) * h / 2);
	C.push_back(0);

	D.push_back(y0 * h);
	for (int i = 0; i < n - 1; i++)
		D.push_back(f(x[i]) * h * h);
	D.push_back(y1 * h);

	y = run_method(A.size(), A, B, C, D);

	return make_pair(x, y);
}

map <string, vector <double>> Runge_Romberg_method(vector <storage> res, vector <double> hs) {
	double k = hs[0] / hs[1];
	vector <double> err_shooting;
	for (int i = 0; i < res[0]["Shooting"]["y"].size(); i++)
		err_shooting.push_back(abs(res[0]["Shooting"]["y"][i] - res[1]["Shooting"]["y"][i]) / (k - 1));

	vector <double> err_fd;
	for (int i = 0; i < res[0]["FD"]["y"].size(); i++)
		err_fd.push_back(abs(res[0]["FD"]["y"][i] - res[1]["FD"]["y"][i]) / (k - 1));

	map <string, vector <double>> err;
	err["Shooting"] = err_shooting;
	err["FD"] = err_fd;
	return err;
}

pair<vector <double>, vector <double>> analytical_solution(double a, double b, double h) {
	vector <double> x, y;
	for (int i = a; i < b + h; i += h)
		x.push_back(i);
	for (int i = 0; i < x.size(); i++)
		y.push_back(exact_func(x[i]));
	return make_pair(x, y);
}

void input(string message, double& d) {
	cout << message;
	cin >> d;
}

int main(int argc, char** argv) {
	
	double a, b, alpha, beta, delta, gamma, st, y0, y1, eps;
	/*input("Enter a: ", a);
	input("Enter b: ", b);
	input("Enter h: ", st);
	input("Enter y0: ", y0);
	input("Enter y_der: ", y_der);*/

	a = 1.1;
	b = 2;
	alpha = 0;
	beta = 1;
	gamma = 1;
	delta = 2;
	y0 = 2;
	y1 = 1;
	st = 0.1;
	y0 = 2;
	eps = 1e-5;


	vector <double> steps = {st, st / 2};

	for (int h = 0; h < steps.size(); h++) {
		vector <double> s_x, s_y;
		auto s_res = shooting_method(a, b, alpha, beta, delta, gamma, y0, y1, steps[h], eps);
		s_x = get<0>(s_res);
		s_y = get<1>(s_res);

		vector <double> fd_x, fd_y, fd_z;
		auto fd_res = finite_difference_method(a, b, alpha, beta, delta, gamma, y0, y1, steps[h]);
		fd_x = get<0>(fd_res);
		fd_y = get<1>(fd_res);

		storage stor;
		stor["Shooting"]["x"] = s_x;
		stor["Shooting"]["y"] = s_y;
		stor["FD"]["x"] = fd_x;
		stor["FD"]["y"] = fd_y;
		save_res.push_back(stor);
	}

	for (int h = 0; h < steps.size(); h++) {
		storage stor;
		for (double i = a; i < b + steps[h]; i += steps[h]) {
			stor["Exact"]["x"].push_back(i);
			stor["Exact"]["y"].push_back(exact_func(i));
		}
		save_res.push_back(stor);
	}

	auto err = Runge_Romberg_method(save_res, steps);


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
 
	glutDisplayFunc(display);
	glutMainLoop();
}