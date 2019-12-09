#include <windows.h>
#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <tuple>
#include <map>
#include <cstdio>
#include "include/matrix.cpp"
#include "include/vector.cpp"
#include "include/LU.cpp"
//#include <libconfig.h++>

#define storage map <string, map <string, vector <double>>>

//методы Ёйлера, –унге- утты и јдамса 4-го пор€дка 

using namespace std;

const double E = 2.71828182845904523536;

const int X_COORD = 50;// X - размерность ] должны
const int Y_COORD = 50;// Y - размерность ] быть равными
const float ITERATIONS = 0.00005;// прорисовка графика (чем меньше тем лучше)

int x_off = X_COORD / 2;// начало
int y_off = Y_COORD / 2;// оси координат

//исходна€ функци€
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
	//	glVertex2d(x_off + x, y_off + j);//не убирать x и y!! это оффсет по ос€м!
	//}
	for (int i = 0; i < x.size() - 1; i++) {
		glVertex2f(x_off + x[i] * 5, y_off + y[i] * 5);
		glVertex2f(x_off + x[i + 1] * 5, y_off + y[i + 1] * 5);
	}
	glEnd();
}


void funcinfo(int val1, int val2) {
	//информаци€ о графике
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
	drawfunc(save_res[0]["Euler"]["x"], save_res[0]["Euler"]["y"], 1);
	drawfunc(save_res[0]["Runge"]["x"], save_res[0]["Runge"]["y"], 2);
	drawfunc(save_res[0]["Adams"]["x"], save_res[0]["Adams"]["y"], 0);
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

double Cauchy_problem(double x, double y, double y_der) {
	return y * cos(x) * cos(x) - y_der * tan(x);
}

double g(double x, double y, double z) {
	return z;
}

double exact_func(double x) {
	return pow(E, sin(x)) + pow(E, -sin(x));
}

pair<vector <double>, vector <double>> analytical_solution(double a, double b, double h) {
	vector <double> x, y;
	for (int i = a; i < b + h; i += h)
		x.push_back(i);
	for (int i = 0; i < x.size(); i++)
		y.push_back(exact_func(x[i]));
	return make_pair(x, y);
}


pair<vector <double>, vector <double>> Euler_method(double a, double b, double h, double y0, double y_der) {
	int n = (b - a) / h;
	vector <double> x, y;
	for (double i = a; i < b + h; i += h) {
		x.push_back(i);
	}
	double z = y_der;
	y.push_back(y0);
	for (int i = 0; i < x.size() - 1; i++) {
		z += h * Cauchy_problem(x[i], y[i], z);
		y.push_back(y[i] + h * g(x[i], y[i], z));
	}
	return make_pair(x, y);
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
		double L1 = h * Cauchy_problem(x[i], y[i], z[i]);
		double K2 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1);
		double L2 = h * Cauchy_problem(x[i] + 0.5 * h, y[i] + 0.5 * K1, z[i] + 0.5 * L1);
		double K3 = h * g(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2);
		double L3 = h * Cauchy_problem(x[i] + 0.5 * h, y[i] + 0.5 * K2, z[i] + 0.5 * L2);
		double K4 = h * g(x[i] + h, y[i] + K3, z[i] + L3);
		double L4 = h * Cauchy_problem(x[i] + h, y[i] + K3, z[i] + L3);
		double delta_y = (K1 + 2 * K2 + 2 * K3 + K4) / 6;
		double delta_z = (L1 + 2 * L2 + 2 * L3 + L4) / 6;
		y.push_back(y[i] + delta_y);
		z.push_back(z[i] + delta_z);
	}
	return make_tuple(x, y, z);
}


pair<vector <double>, vector <double>> Adams_method(vector <double> x, vector <double> y, vector <double> z, double h) {
	double n = x.size();
	x.resize(4);
	y.resize(4);
	z.resize(4);
	for (int i = 3; i < n - 1; i++) {
		double z_i = z[i] + h * (55 * Cauchy_problem(x[i], y[i], z[i]) -
			59 * Cauchy_problem(x[i - 1], y[i - 1], z[i - 1]) +
			37 * Cauchy_problem(x[i - 2], y[i - 2], z[i - 2]) -
			9 * Cauchy_problem(x[i - 3], y[i - 3], z[i - 3])) / 24;
		z.push_back(z_i);
		double y_i = y[i] + h * (55 * g(x[i], y[i], z[i]) -
				59 * g(x[i - 1], y[i - 1], z[i - 1]) +
				37 * g(x[i - 2], y[i - 2], z[i - 2]) -
				9 * g(x[i - 3], y[i - 3], z[i - 3])) / 24;
		y.push_back(y_i);
		x.push_back(x[i] + h);
	}
	return make_pair(x, y);
}


map <string, vector <double>> Runge_Romberg_method(vector <storage> res, vector <double> hs) {
	double k = hs[0] / hs[1];
	vector <double> err_euler;
	for (int i = 0; i < res[0]["Euler"]["y"].size(); i++)
		err_euler.push_back(abs(res[0]["Euler"]["y"][i] - res[1]["Euler"]["y"][i]) / (k - 1));

	vector <double> err_runge;
	for (int i = 0; i < res[0]["Runge"]["y"].size(); i++)
		err_runge.push_back(abs(res[0]["Runge"]["y"][i] - res[1]["Runge"]["y"][i]) / (pow(k, 4) - 1));

	vector <double> err_adams;
	for (int i = 0; i < res[0]["Adams"]["y"].size(); i++)
		err_runge.push_back(abs(res[0]["Adams"]["y"][i] - res[1]["Adams"]["y"][i]) / (pow(k, 4) - 1));

	map <string, vector <double>> err;
	err["Euler"] = err_euler;
	err["Runge"] = err_runge;
	err["Adams"] = err_adams;
	return err;
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

void input(string message, double& d) {
	cout << message;
	cin >> d;
}

int main(int argc, char** argv) {
	
	auto f = freopen("log.txt", "w", stdout);
	double a, b, st, y0, y_der;
	/*input("Enter a: ", a);
	input("Enter b: ", b);
	input("Enter h: ", st);
	input("Enter y0: ", y0);
	input("Enter y_der: ", y_der);*/

	a = 0;
	b = 1;
	st = 0.1;
	y0 = 2;
	y_der = 0;

	printf("Interval: [%f, %f]\n", a, b);
	printf("h=%f, y0=%f, y_der=%f\n", st, y0, y_der);
	vector <double> steps = {st, st / 2};

	for (int h = 0; h < steps.size(); h++) {
		printf("Euler method:\n");
		vector <double> euler_x, euler_y;
		auto euler_res = Euler_method(a, b, steps[h], y0, y_der);
		euler_x = get<0>(euler_res);
		euler_y = get<1>(euler_res);
		for (int i = 0; i < euler_x.size(); i++)
			printf("x[%d]=%f\ty[%d]=%f\n", i, euler_x[i], i, euler_y[i]);

		printf("Runge-Kutta method:\n");
		vector <double> runge_x, runge_y, runge_z;
		auto runge_res = Runge_Kutta_method(a, b, steps[h], y0, y_der);
		runge_x = get<0>(runge_res);
		runge_y = get<1>(runge_res);
		runge_z = get<2>(runge_res);
		for (int i = 0; i < runge_x.size(); i++)
			printf("x[%d]=%f\ty[%d]=%f\n", i, runge_x[i], i, runge_y[i]);

		printf("Adams method:\n");
		vector <double> adams_x, adams_y;
		auto adams_res = Adams_method(runge_x, runge_y, runge_z, steps[h]);
		adams_x = get<0>(adams_res);
		adams_y = get<1>(adams_res);
		for (int i = 0; i < adams_x.size(); i++)
			printf("x[%d]=%f\ty[%d]=%f\n", i, adams_x[i], i, adams_y[i]);

		storage stor;
		stor["Euler"]["x"] = euler_x;
		stor["Euler"]["y"] = euler_y;
		stor["Runge"]["x"] = runge_x;
		stor["Runge"]["y"] = runge_y;
		stor["Adams"]["x"] = adams_x;
		stor["Adams"]["y"] = adams_y;
		save_res.push_back(stor);
	}
	auto errs = Runge_Romberg_method(save_res, steps);
	printf("errors:\n");
	printf("Euler:\n");
	for (int i = 0; i < errs["Euler"].size(); i++) {
		printf("[%d] %f\n", i, errs["Euler"][i]);
	}
	printf("Runge:\n");
	for (int i = 0; i < errs["Runge"].size(); i++) {
		printf("[%d] %f\n", i, errs["Runge"][i]);
	}
	printf("Adams:\n");
	for (int i = 0; i < errs["Adams"].size(); i++) {
		printf("[%d] %f\n", i, errs["Adams"][i]);
	}

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