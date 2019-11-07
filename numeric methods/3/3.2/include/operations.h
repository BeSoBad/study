#ifndef OPERATIONS
#define OPERATIONS

#include "vector.h"
#include "matrix.h"

Vector operator*(Matrix& A, Vector& B);
Matrix operator*(Matrix& A, Matrix& B);
Matrix operator*(Vector& A, Vector& B);
Matrix operator*(Matrix& A, double d);
Matrix operator*(double d, Matrix& A);
Vector operator+(Vector& A, Vector& B);
Vector operator-(Vector& A, Vector& B);
Matrix operator-(Matrix& A, Matrix& B);
Matrix operator+(Matrix& A, Matrix& B);

#endif
