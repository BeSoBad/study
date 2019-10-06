#ifndef VECTOR
#define VECTOR

#include <iostream>
#include "vector.h"
class Vector {
public:
	vector <double> x;
	int n;
	double& operator[](int i);
	Vector(int nn);
	void show();
	int size();
	void push(double t);
  double norm();
  void readVector(string &&path);
};

#endif
