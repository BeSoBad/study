#ifndef OPERATIONS
#define OPERATIONS

#include "vector.h"
#include "matrix.h"

Vector operator*(Matrix& A, Vector& B);
Matrix operator*(Matrix& A, Matrix& B);
Vector operator+(Vector& A, Vector& B);
Vector operator-(Vector& A, Vector& B);
Matrix operator-(Matrix& A, Matrix& B);
Matrix operator+(Matrix& A, Matrix& B);

#endif
