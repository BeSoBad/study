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

vector <double> points_g, F_g_1, F_g_2, F_g_3;
double color_g_1, color_g_2, color_g_3;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "Osnovnie toshki po vashemu grafiku: \n";
	//vector <double>& points, vector <double>& F, double color

	drawgrid(0.3, 5);
	drawfunc(points_g, F_g_1, color_g_1);
	drawfunc(points_g, F_g_2, color_g_2);
	drawfunc(points_g, F_g_3, color_g_3);
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

double F(vector <double>& coefs, double x) {
	double sum = 0;
	for (int i = 0; i < coefs.size(); i++)
		sum += pow(x, i) * coefs[i];
	return sum;
}

double sum_squared_errors(vector <double> F, vector <double> y) {
	double sum = 0;
	for (int i = 0; i < F.size(); i++)
		sum += pow(F[i] - y[i], 2);
	return sum;
}

Vector MLS(double n, vector <double> x, vector <double> y) {
	int N = x.size();
	Matrix mat(n + 1, n + 1);
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			double sum = 0;
			for (int k = 0; k < x.size(); k++)
				sum += pow(x[k], i + j);
			mat[i][j] = sum;
		}
	}
	mat[0][0] = N + 1;
	Vector b(n + 1);
	for (int i = 0; i < n + 1; i++) {
		double sum = 0;
		for (int j = 0; j < N; j++)
			sum += pow(x[j], i) * y[j];
		b[i] = sum;
	}
	Matrix L(n + 1, n + 1), U(n + 1, n + 1);
	LU(mat, L, U, n + 1);
	return solve(L, U, b);
}
