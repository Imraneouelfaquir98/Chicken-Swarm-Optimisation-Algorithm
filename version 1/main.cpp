#include <iostream>
#include "cso.h"

using namespace std;

float cost(float x[], int size_array){
	float t = 0;
	for(int i=0; i<size_array; i++) t=t+x[i]*x[i]*x[i]*x[i]+1;
		return t;
}

int main(int argc, char const *argv[])
{
	float C1[5] = {-5.12, -5.12, -5.12, -5.12, -5.12};
	float C2[5] = { 5.12,  5.12,  5.12,  5.12,  5.12};

	CSO cso = CSO( 5, C1, C2, &cost);
	cso.fit();
    cout<<"Hello word "<<endl;
    return 0;
}