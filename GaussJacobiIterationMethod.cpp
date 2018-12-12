#include "pch.h"
#include <math.h>
#include <iostream>
#include <string> 
#include <vector>
#include <stdio.h>
#include <fstream>
//#include <algorithm>

using namespace std;

class Jacobi {
public: Jacobi(int n, vector<vector<double>>& A, vector<double>& b, double eps)
{
	vector<double> x(n);
	vector<double> T(n);
	double k = 0;
	int loops = 0;
	x.resize(n, 0);
	T.resize(n, 0);

	do {
		for (int i = 0; i < n; i++) {
			T[i] = b[i];
			for (int j = 0; j < n; j++) {
				if (i != j) {
					if (A[i][j] == 0) {
						continue;
					}
					T[i] -= A[i][j] * x[j];
				}
			}
			T[i] /= A[i][i];
		}
		k = fabs(x[0] - T[0]);
		for (int h = 0; h < n; h++) {
			if (fabs(x[h] - T[h]) > k)
				k = fabs(x[h] - T[h]);
			x[h] = T[h];
		}
		loops++;
	} while (k > eps);

	cout << endl << "Solution: " << endl;

	for (int g = 0; g < n; g++) {
		cout << endl << "x(" << g << ") = " << x[g] << endl;
	}

	cout << endl << "Iterations: " << loops << endl;

	/* OUT */
	///*
	ofstream myfile("C:\\Users\\Asus\\Desktop\\out.txt");

	if (myfile.is_open())
	{
		myfile << "Solution:" << endl << endl;
		for (int i = 0; i < n; i++) {
			myfile << "x(" << i << ") = " << x[i] << endl;
		}
		myfile << endl << "Iterations: " << loops << endl << "eps:" << eps << endl;
		myfile.close();
		cout << endl << "The solution file 'out.txt' is generated!" << endl;
	}
	else cout << "Unable to open file";
	//*/
}
};

int main()
{

	/****************************************************************

	  10.0*x1 + -1.0*x2 + 2.0*x3 + 0.0*x4 = 6.0
	 -1.0*x1 + 11.0*x2 + -1.0*x3 + 3.0*x4 = 25.0
	  2.0*x1 + -1.0*x2 + 10.0*x3 + -1.0*x4 = -11.0
	  0.0*x1 + 3.0*x2 + -1.0*x3 + 8.0*x4 = 15.0
	  ~ Solution : [1.          1.99999999 - 0.99999999  0.99999999]
	  = Solution : [1.          2.         - 1.          1.        ]

	 ****************************************************************/

	 //A = {{10,-1,2,0},{-1,11,-1,3},{2,-1,10,-1},{0,3,-1,8}};
	 //b = { 6,25,-11,15 };


	int n = 0; // 4
	double eps = 0.; // 0.000001
	char A_file[100]; // C:\\Users\\Asus\\Desktop\\matrix_A.txt
	char b_file[100];
	//char output_file[100];

	cout << "Enter the n: "; cin >> n;
	cout << "Enter the Matrix A file name: \n(e.g. C:\\Users\\Asus\\Desktop\\matrix_A.txt)"; cin >> A_file;
	cout << "Enter the Right part file name: \n(e.g. C:\\Users\\Asus\\Desktop\\matrix_b.txt)"; cin >> b_file;
	cout << "Enter the eps: "; cin >> eps;
	//cout << "Enter the output file name: \n(e.g. C:\\Users\\Asus\\Desktop\\out.txt)"; cin >> output_file; 


	vector<vector<double>> A(n);
	vector<double> b(n);

	for (int i = 0; i < n; i++) {
		A[i].resize(n);
	}

	b.resize(n);
	ifstream A_input_file(A_file);
	ifstream b_input_infile(b_file);


	if (!A_input_file || !b_input_infile) {
		cout << "Can't open the file.\n";
	}

	for (int i = 0; i < n; i++) {
		b_input_infile >> b[i];
		for (int j = 0; j < n; j++) {
			A_input_file >> A[i][j];
		}
	}


	A_input_file.close();
	b_input_infile.close();

	new Jacobi(n, A, b, eps);
}