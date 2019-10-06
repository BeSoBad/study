#include "vector.h"
#include "matrix.h"

Vector operator*(Matrix& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			C[i] += A[i][j] * B[j];
	return C;
}

Matrix operator*(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			for(int k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];
	return C;
}

Vector operator+(Vector& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		C[i] = A[i] + B[i];
	return C;
}

Vector operator-(Vector& A, Vector& B) {
	int n = A.size();
	Vector C(n);
	for(int i = 0; i < n; i++)
		C[i] = A[i] - B[i];
	return C;
}

Matrix operator-(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] - B[i][j];
	return C;
}

Matrix operator+(Matrix& A, Matrix& B) {
	int n = A.size();
	Matrix C(n, n);
	for(int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] + B[i][j];
	return C;
}
