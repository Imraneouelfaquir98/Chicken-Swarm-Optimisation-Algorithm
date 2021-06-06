#ifndef CSO_H
#define CSO_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

class CSO{
public:
	double FL_MAX=0.9, e = 1e-8, FL_MIN= 0.5;

	int D = 5, N = 10, G = 10, RP = 20, HP = 60;
	int TheBest = 0, MAX = 40, cType[10], cPositionInSwarm[10];

	float Cmin[5], Cmax[5];
	float C[10][5], E[10], Gbest[5], EGbest;

	CSO(
		int D,
		float Cmin[5],
		float Cmax[5],
		float (*func)(float x[], int size_array)
	);
	
	float (*cost_func)(float x[], int size_array);
	void fit();
};

#endif // CSO_H