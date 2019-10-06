double& Vector::operator[](int i) {
	return x[i];
}
Vector::Vector(int nn) {
	vector <double> tmp(nn);
	x = tmp;
	n = nn;
}
void Vector::show() {
	for (int i = 0; i < n; i++) {
		cout << x[i] << "\t";
	}
	cout << endl;
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

void Vector::readVector(string &&path) {
	ifstream in(path);

	if (in.is_open()) {
		int count = 0;
		double temp;
		Vector tmp(0);
		while (!in.eof()) {
				in >> temp;
				tmp.push(temp);
		}
		tmp.n--;
		x = tmp;
	}
	else {
		cout << "Файл не найден.";
	}
}
