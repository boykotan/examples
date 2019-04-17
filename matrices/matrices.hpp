#pragma once

double* newmatrix(const size_t row,  const size_t col);
void print(const size_t row, const size_t col, const double *matrix);
double* add(const size_t row_m1, const size_t col_m1, double *matrix1, std::ifstream &matrix2);
double* mult(const size_t row_m1, size_t &col_m1, double *matrix1, std::ifstream &matrix2);

// Declarations of functions for working with matrices goes here.
