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
  double max = 0;
  for (int i = 0; i < n; i++) {
    double s = 0.0;
    s += abs(x[i]);
    if (s > max)
      max = s;
  }
  return max;
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
		tmp.n--;
		this->x = tmp.x;
		this->n = tmp.n;
	}
	else {
		std::cout << "Файл не найден.";
	}
}
