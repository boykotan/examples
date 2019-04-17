#include <iostream>
#include <fstream>
#include <string>
#include "matrices.hpp"

using namespace :: std;

bool is_opened_file (const ifstream &file, const char *name) {
	if (static_cast<bool>(file)) {
		return true;
	}
	cerr << "Can't open file: " << name << endl;
	return false;
}

int main (int argc, char** argv) {
	if (argc == 1) {
		cerr << "Usage: ./main file.txt --add|--mult file2.txt" << endl;
		return -6;
	}
	if (argc % 2 == 1) {
		cerr << "Incorrect input. Usage: ./main file.txt --add|--mult file2.txt" << endl;
		return -7;
	}
	ifstream workfile(argv[1]);
	if (!is_opened_file(workfile, argv[1])) {
		return -1;
	}
	size_t row, col;
	workfile >> row >> col;
	double *res;
	if ((res = newmatrix(row, col)) == nullptr) {
		return -6;
	}
	for (size_t i = 0; i < row * col; i++) {
		workfile >> res[i];
	}
	for (int i = 2; i < argc; i += 2) {
		ifstream workfile(argv[i + 1]);
		if (!is_opened_file(workfile, argv[i + 1])) {
			delete[] res;
			return -1;
		}
		if (string(argv[i]) == "--add") {
			res = add(row, col, res, workfile);
			if (res == nullptr) {
				cerr << "At operation <accumulated_matrix> add <" << argv[i + 1] << ">" << endl;
				return -3;
			}
		}
		else if (string(argv[i]) == "--mult") {
			res = mult(row, col, res, workfile);
			if (res == nullptr) {
				cerr << "At operation " << argv[i - 1] << " mult " << argv[i + 1] << endl;
				return -4;
			}
		}
		else {
			cerr << "Invalid operation: " << argv[i] << endl;
			delete[] res;
			return -5;
		}
	}
	print(row, col, res); 
	delete[] res;
	return 0;
}
