#include <iostream>
#include "matrix.h"
Matrix::Matrix(int nn, int mm) {
    std::vector <std::vector <double>> tmp(nn, std::vector <double> (mm));
    x = tmp;
    n = nn;
    m = mm;
    for (int i = 0; i < n; i++)
        x[i][i] = 1;       
}

std::vector <double>& Matrix::operator[](int i) {
    return x[i];
}
void Matrix::show() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            std::cout << x[i][j] << "\t";
        std::cout << "\n";
    }
    std::cout << std::endl;
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

void Matrix::readMatrix(std::string &path) {
	std::ifstream in(path);
    std::cout << path << std::endl;
    in.seekg(0);
	if (in.is_open()) {
		int count = 0;
		double temp;
		std::vector <double> temp_v;
		while (!in.eof()) {
				in >> temp;
				temp_v.push_back(temp);
				count++;
		}
		this->n = std::sqrt(count);
        this->m = this->n;
		Matrix tmp(n, n);
		int cnt = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (cnt >= count) {
					break;
				}
				tmp[i][j] = temp_v[cnt];
				cnt++;
			}
			if (cnt >= count) {
				break;
			}
		}
		this->x = tmp.x;
	}
	else {
		std::cout << "Файл не найден.";
	}
}

void Matrix::operator=(Matrix& b) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            x[i][j] = b[i][j];
        }
    }
}