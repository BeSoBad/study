#include "vector.h"

double& Vector::operator[](int i) {
	return x[i];
}
Vector::Vector(int nn) {
	std::vector <double> tmp(nn);
	x = tmp;
	n = nn;
}
void Vector::show() {
	for (int i = 0; i < n; i++) {
		std::cout << x[i] << "\t";
	}
	std::cout << std::endl;
}
int Vector::size() {
	return n;
}
void Vector::push(double t) {
	x.push_back(t);
	n++;
}

double Vector::norm() {
	double res = 0;
	for (int i = 0; i < n; i++) {
		res += x[i] * x[i];
	}
	return std::sqrt(res);
}

void Vector::readVector(std::string &&path) {
	std::ifstream in(path);

	if (in.is_open()) {
		int count = 0;
		double temp;
		Vector tmp(0);
		while (!in.eof()) {
				in >> temp;
				tmp.push(temp);
				count++;
		}
		this->x = tmp.x;
		this->n = tmp.n;
	}
	else {
		std::cout << "Файл не найден.";
	}
}
