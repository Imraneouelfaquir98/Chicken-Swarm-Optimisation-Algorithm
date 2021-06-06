#include <iostream>
#include <vector>
#include "cso.h"

using namespace std;

float cost(vector<float> x){
	float t = 0;
	for(int i=0; i<(int)x.size(); i++) t=t+x[i]*x[i]*x[i]*x[i]+1;
		return t;
}

int main(int argc, char const *argv[])
{
	vector<float> C1 (5,-5.12);
	vector<float> C2 (5, 5.12);

	CSO cso = CSO(
		5,    // D = Dimension
		10,   // N 
		10,   // G
		40,   // Max iteration
		C1,   // Cmin
		C2,   // Cmax
		&cost // Cost function
	);

	cso.fit();
    cout<<"Hello word "<<endl;
    return 0;
}