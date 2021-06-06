#include "cso.h"
#include <iostream>
#include <ctime>
#include <math.h>
#include <vector>

using namespace std;

double rand(double min, double max){
	return min + (double) rand() / RAND_MAX * (max - min);
}

float r(){
	return (float) (rand()%1000)/1000;
}


CSO::CSO(
	int D,
	int N,
	int G,
	int max_iter,
	vector<float> Cmin,
	vector<float> Cmax,
	float (*func)(vector<vector<float>> X, vector<float> y, vector<float> W)
)
{
	this->D   = D;
	this->N   = N;
	this->G   = G;
	this->MAX = max_iter;
	cost_func = func;

	this->Cmax.resize(D, 0);
	this->Cmin.resize(D, 0);
	for(int i = 0; i < D; i++)
	{
		this->Cmax[i] = Cmax[i];
		this->Cmin[i] = Cmin[i];
	}

	this->nRoosters = (this->N*this->RP)/100;
	this->nHens     = (this->N*this->HP)/100;
	this->nChicks   =  this->N-this->nRoosters-this->nHens;

	this->hens.resize    (this->nHens    , 0);
	this->chicks.resize  (this->nChicks  , 0);
	this->roosters.resize(this->nRoosters, 0);

	this->cType.resize(this->N, 0);
	this->cPositionInSwarm.resize(this->N, 0);

	this->E.resize    (this->N, 0);
	this->Gbest.resize(this->D, 0);
	this->C.resize    (this->N, vector<float>(this->D, 0));
}

void CSO::fit(vector<vector<float>> X, vector<float> y){
	this->X = X; this->y = y;
	this->D = (int)X[0].size();

	unordered_map<int, int> hRoosterRelation;
	unordered_map<int, int> cHenRelation;

	for (int i = 0; i < N; ++i) this->cPositionInSwarm[i] = i;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < D; ++j) C[i][j] = (Cmax[j] - Cmin[j]) * r() + Cmin[j];
		E[i] = cost_func(X, y, C[i]);
		if(E[i] < E[TheBest]) TheBest = i;
	}

	for(int j=0; j<D; j++) Gbest[j] = C[TheBest][j];

	EGbest = E[TheBest];
	
	int t=0;
	while(t<MAX){
		if(t%G == 0){
			vector<double> f_values;
			for(int i=0; i<N; i++) f_values.push_back(E[i]);
			for(int i=0; i<N; i++) cType[i] = 0;
			sort(f_values.begin(), f_values.end());
			double t_roosters = f_values[nRoosters-1];
			double t_hens = f_values[nRoosters+nHens-1];
			int rIndex = 0, hIndex = 0, cIndex = 0;
			for (int i = 0; i < N; ++i)
			{
				if(E[i] <=t_roosters){
					cType[i] = 2;
					roosters[rIndex++] = i;
					if( rIndex >= nRoosters) break;
				}
			}
			for ( int i = 0 ; i < N ; i ++) {
				if (E[i] <= t_hens && cType[i] != 2 ){
					cType[i] = 1; 
					hens[hIndex++] = i;
					if ( hIndex >= nHens ) 
						break ;
				}
			}
			for(int i = 0 ; i<N ; i++)
				if(cType[i] == 0 )
					chicks[cIndex++] = i;
			for(int i = 0 ; i<nHens ; i++) {
				int hIndex = hens[i]; 
				int r = rand()%nRoosters;
				int rIndex = roosters[r]; 
				hRoosterRelation [hIndex]=rIndex ;
			}
			for(int i = 0 ; i<nChicks ; i++) {
				int cIndex=chicks[i];
				int hen = rand()%nHens;
				int hIndex = hens[hen];
				cHenRelation[cIndex] = hIndex;
			}
		}
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < D; ++j)
			{
				if(cType[i] == 2 ) {
					double sigma_squared=1;int k = rand()%nRoosters;
					if(E[k]<E[i]) sigma_squared = exp((E[k]-E[i])/(abs(E[i])+e));
					default_random_engine generator;
					normal_distribution <double>d(0,sigma_squared);
					C[i][j]=C[i][j]*(1+d(generator));
				}
				else if(cType[i] == 1){
					int r1 = hRoosterRelation[i]; int r2 = r1;
					while(r2 != r1){
						int type = rand()%2;
						if(type == 0){
							r2 = roosters[rand() % nRoosters];
						}else{
							r2 = hens[rand()%nHens];
						}
					}
					double s1 = exp((E[i]-E[r1])/(abs(E[i])+e));
					double s2 = exp(E[r2]-E[i]);
					C[i][j]=C[i][j]+s1*rand(0,1)*(C[r1][j]-C[i][j])+rand(0,1)*s2*(C[r2][j]-C[i][j]);
				}
				else{
					int m = cHenRelation[i];
					C[i][j] = C[i][j] + rand(FL_MIN, FL_MAX) * (C[m][j] - C[i][j]);
				}
				if(C[i][j]< Cmin[j]) C[i][j] = Cmin[j];
				if(C[i][j]> Cmax[j]) C[i][j] = Cmax[j];
			}
		}
		TheBest = 0;
		for(int i = 0 ; i<N ; i++) {
			E[i] = cost_func(X, y, C[i]);
			TheBest = (E[i]<E[TheBest])?i:TheBest;
		}
		if(E[TheBest]<EGbest){
			EGbest = E[TheBest];
			for(int j=0 ;j<D;j++) Gbest[j]=C[TheBest][j];
		}
		t++;
		if(t%10 == 0)
			cout<<t
				<<"- "
				<<"Cost function = "
				<<(float)EGbest<<endl;
	}
}