#include <iostream>
#include "matrix.h"
Matrix::Matrix(int nn, int mm) {
    vector <vector <double>> tmp(nn, vector <double> (mm));
    x = tmp;
    n = nn;
    m = mm;
}
vector <double>& Matrix::operator[](int i) {
    return x[i];
}
void Matrix::show() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << x[i][j] << "\t";
        cout << "\n";
    }
    cout << endl;
}
int Matrix::size() {
    return n;
}
void Matrix::transpose() {
    int t;
    Matrix tmp(n, n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            tmp[i][j] = x[j][i];
        }
    }
    x = tmp.x;
}
double Matrix::norm() {
    double max = 0;
    for (int i = 0; i < n; i++) {
        double s = 0.0;
        for (int j = 0; j < n; j++) {
            s += abs(x[i][j]);
        }
        if (s > max)
            max = s;
    }
    return max;
}

void Matrix::readMatrix(string &&path) {
	ifstream in(path);

	if (in.is_open()) {
		int count = 0;
		int temp;
		vector <int> temp_v;
		while (!in.eof()) {
				in >> temp;
				temp_v.push_back(temp);
				count++;
		}
		int n = sqrt(count);
		Matrix tmp(n, n);
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (cnt >= count) {
					break;
				}
				in >> temp;
				tmp[i][j] = temp_v[cnt];
				cnt++;
			}
			if (cnt >= count) {
				break;
			}
		}
		x = tmp;
	}
	else {
		cout << "Файл не найден.";
	}
}
