#include "pch.h"
#include <math.h>
#include <iostream>
#include <string> 
#include <vector>
#include <stdio.h>
#include <fstream>
//#include <algorithm>

using namespace std;


int main()
{
	int n = 3;
	vector<double> R(n), C(n), V(n);
	vector<double> b(n), r(n);


	R.resize(n);
	C.resize(n);
	V.resize(n);
	b.resize(n);
	r.resize(n);

	/* 
	1 2 0    5
	0 2 0    4
	0 0 5   15
	*/  
	
	V = {1,2,2,5}; 
    R = {0,2,3,4}; 
	C = {0,1,1,2};

	b = {1,2,3};

	
	for (int i = 0; i < n; i++)
		{
			for (int k = R[i]; k < R[i + 1]; k++)
			{
				r[i] = r[i] + V[k] * b[C[k]];
			}
			cout << r[i] << endl;
		}
	
	 
	
}