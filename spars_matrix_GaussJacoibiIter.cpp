//#include "pch.h"
#include <math.h>
#include <iostream>
#include <string> 
#include <vector>
#include <stdio.h>
#include <fstream>
//#include <algorithm>

using namespace std;

void Jacobi(int n, vector<int>& starts,vector<int>& cols, vector<double>& vals,vector<double>& b, double eps, char *output_file) {
	vector<double> x(n);
	vector<double> T(n);
	double k = 0; // k > eps
	int D = 0; // Diagonal
	int loops = 0;
	x.resize(n, 0);
	T.resize(n, 0);
	
	#pragma omp parallel
	{

	 do {
		
		 for (int i = 0; i < n ; i++ ) {
			 T[i] = b[i];
			 for (int j = starts[i]; j < starts[i+1]; j++ ) {
				 if (i != cols[j]) {
					 T[i] -= vals[j] * x[cols[j]];
					 }else{
						 D = j;
						 }
				}
		      T[i] /= vals[D];
		    }

		 #pragma omp critical
		 {
			
		  k = fabs(x[0] - T[0]);
		  for (int h = 0; h < n; h++) {
			  if (fabs(x[h] - T[h]) > k)
			 	  k = fabs(x[h] - T[h]);
			  x[h] = T[h];
		  }
		
		 } //END OF omp critical
		
	 } while (k > eps);
		
	} //END OF pragma
	
	/* GENERATING THE OUTPUT FILE */
	
	ofstream out_file(output_file);

	if ( out_file.is_open()) {
		for (int i = 0; i < n; i++) {
			out_file << endl << x[i] << endl;
		}
		out_file.close();
		cout << endl << "The solution file is generated!" << endl;
	} else cout << "Unable to open the file";
}

int main(int argc, char* argv[])
{

	/****************************************************************
	  10.0*x1 + -1.0*x2 + 2.0*x3 + 0.0*x4 = 6.0
	 -1.0*x1 + 11.0*x2 + -1.0*x3 + 3.0*x4 = 25.0
	  2.0*x1 + -1.0*x2 + 10.0*x3 + -1.0*x4 = -11.0
	  0.0*x1 + 3.0*x2 + -1.0*x3 + 8.0*x4 = 15.0
	  ~ Solution : [1.          1.99999999 - 0.99999999  0.99999999]
	  = Solution : [1.          2.         - 1.          1.        ]
	  ---------------------------------------------------------------
	  A = {{10,-1,2,0},{-1,11,-1,3},{2,-1,10,-1},{0,3,-1,8}};
	  b = { 6,25,-11,15 };
	  int n = 4;
	 ****************************************************************/
	
	int n;
	char *A_file = argv[1];
	char *b_file = argv[2];
	double eps = atof(argv[3]);
	char *output_file = argv[4];

	ifstream A_input_file(A_file);
	ifstream b_input_infile(b_file);

	if (!A_input_file || !b_input_infile) {
		cout << "Can't open the file.\n";
	}
	
	A_input_file >> n;
	vector<double> b(n);

	for (int i = 0; i < n; i++ ) {
		b_input_infile >> b[i];
		}
		
    /*
    vector<vector<double>> A(n);
	for (int i = 0; i < n; i++) {
		A[i].resize(n);
	}
	*/
	
	vector<int> starts(n+1); 
	for (int i = 0; i < n+1; i++ ) {
		A_input_file >> starts[i];
		} 
		
	vector<int> cols(starts[n]);
	for (int i = 0; i < starts[n]; i++ ) {
		A_input_file >> cols[i];
		}
		
	vector<double> vals(starts[n]);
	for (int i = 0; i < starts[n]; i++ ) { 
	A_input_file >> vals[i];
	}
	
	/********* Rebuilding the dense matrix back *********
	for (int i = 0; i < n ; i++ ) {	
		for (int j = starts[i]; j < starts[i+1]; j++ ) {
			A[i][cols[j]] = vals[j];
			}	
	}
    ****************************************************/

	A_input_file.close();
	b_input_infile.close();
	Jacobi(n, starts, cols, vals, b, eps, output_file);
}
