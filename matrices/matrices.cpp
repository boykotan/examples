#include <iostream>
#include <iomanip>
#include <fstream>

#include "matrices.hpp"


using namespace :: std;

double* newmatrix(const size_t row, const size_t col) {
	double *res;
	try {
		res = new double[row * col];
	}
	catch (const bad_alloc& e) {
		cerr << "I have not so much memory: " << row * col * sizeof(double) << "byte(s)" << endl;
		return nullptr;
	}
	return res;
}

void print(const size_t row, const size_t col, const double *matrix) {
	cout << row << " " << col << endl;
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            cout << setw(sizeof(double)) << matrix[i * col + j] << " ";
        }
        cout << endl;
    }
}

double* add(const size_t row_m1, const size_t col_m1, double *matrix1, ifstream &in) {
    size_t row_m2, col_m2;
    in >> row_m2 >> col_m2;
    if ((row_m1 != row_m2) || (col_m1 != col_m2)) {
        cerr << "bad matrix size" << endl;
        delete[] matrix1;
        return nullptr;
    }
    for (size_t i = 0; i < row_m1; i++) {
        for (size_t j = 0; j < col_m1; j++) {
            double m_2;
            in >> m_2;
            matrix1[i * col_m1 + j] += m_2;
        }
    }
    return matrix1;
}

double* mult(const size_t row_m1, size_t &col_m1, double *matrix1, ifstream & in) {
    size_t row_m2, col_m2;
    in >> row_m2 >> col_m2;
    if (col_m1 != row_m2) {
        cerr << "bad matrix size: " << col_m1 << " != " << row_m2 << endl;
        delete[] matrix1;
        return nullptr;
    }
	
	double *matrix2;
	if ((matrix2 = newmatrix(row_m2, col_m2)) == nullptr) {
		delete[] matrix1;
		return nullptr;
	}
	for (size_t i = 0; i < row_m2 * col_m2; i++)
		in >> matrix2[i];
	
    double *res;
	if ((res = newmatrix(row_m1, col_m2)) == nullptr) {
		delete[] matrix1;
		delete[] matrix2;
		return nullptr;
	}
    for (size_t i = 0; i < row_m1; i++) {
        for (size_t j = 0; j < col_m2; j++) {
			double sum = 0.0;
			for (size_t k = 0; k < col_m1; k++) {
				sum += matrix1[i * col_m1 + k] * matrix2[k * col_m2 + j];
			}
            res[i * col_m2 + j] = sum;
        }
    }
	col_m1 = col_m2;
	delete[] matrix1;
    delete[] matrix2;
    return res;
}
