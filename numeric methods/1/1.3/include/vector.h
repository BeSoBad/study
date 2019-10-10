#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Vector {
public:
	std::vector <double> x;
	int n;
	double& operator[](int i);
	Vector(){}
	Vector(int nn);
	void show();
	int size();
	void push(double t);
  double norm();
  void readVector(std::string &&path);
};

#endif
