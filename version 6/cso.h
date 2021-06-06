#ifndef CSO_H
#define CSO_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <bits/stdc++.h>
#include <math.h>
#include <vector>

using namespace std;

class CSO{
public:
	double FL_MAX=0.9, e = 1e-8, FL_MIN= 0.5;

	int D, N, G, RP = 20, HP = 60;
	int TheBest = 0, MAX;
	vector<int> cType, cPositionInSwarm;

	float EGbest;
	vector<float> Cmin, Cmax, E, Gbest;
	vector<vector<float>> C;

	int nRoosters, nHens, nChicks;
	vector<int> hens, chicks, roosters;

	vector<vector<float>> X;
	vector<float> y;

	CSO(
		int D,
		int N,
		int G,
		int max_iter,
		vector<float> Cmin,
		vector<float> Cmax,
		float (*func)(vector<vector<float>> X, vector<float> y, vector<float> W)
	);
	
	float (*cost_func)(vector<vector<float>> X, vector<float> y, vector<float> W);
	void fit(vector<vector<float>> X, vector<float> y);
};

#endif // CSO_H