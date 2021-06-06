#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include "cso.h"

using namespace std;

vector<float> image_to_vector(cv::Mat image);
float         sigmoid        (vector<float> X         , vector<float> W);
float         cost           (vector<vector<float>>  X, vector<float> y, vector<float> W);
void          load_data      (vector<vector<float>> *X, vector<float>*y);
int predict(vector<float> X, vector<float> W);

int main(int argc, char const *argv[])
{

	vector<vector<float>> X;
	vector<float> y;
	load_data(&X, &y);

	vector<float> C1 (40001,-10);
	vector<float> C2 (40001, 10);

	CSO cso = CSO(
		40001,// D = Dimension
		20,   // N 
		 5,   // G
		100,   // Max iteration
		C1,   // Cmin
		C2,   // Cmax
		&cost // Cost function
	);
	cso.fit(X, y);


	for (int i = 0; i < (int)X.size(); ++i)
		cout<<"y = "
				<<y[i]
				<<", y_pred = "
				<<predict(X[i], cso.Gbest)
				<<", sigmoid = "
				<<sigmoid(X[i], cso.Gbest)<<endl;

	for (int i = 0; i < (int)cso.Gbest.size(); ++i)
		if (cso.Gbest[i] !=0 )
			cout<<cso.Gbest[i]<<endl;

	// for(int i=0; i<(int)X[0].size(); i++)
	// 	cout<<X[0][i]<<endl;
	// cout<<X[0].size()<<endl;
  return 0;
}


vector<float> image_to_vector(cv::Mat image){
  vector<float> x;
  x.push_back(1);
  for (int i = 0; i < image.rows; ++i)
  	for (int j = 0; j < image.cols; ++j)
  		x.push_back((float)image.at<uchar>(i, j));
  return x;
}

float sigmoid(vector<float> X, vector<float> W){
	float val = 0;
	for (int i = 0; i < (int)X.size(); ++i)
		val += (float) X[i] * W[i];
	return 1/(1 + exp(-val));
}

float cost(vector<vector<float>> X, vector<float> y, vector<float> W){
	float val = 0;
	for(int i=0; i<(int)X.size(); i++) 
		val += (float)pow(sigmoid(X[i], W) - y[i], 2);
	return (float)val/X.size();
}

void load_data(vector<vector<float>> *X, vector<float> *y){
	cv::Mat image;
	for (int k = 0; k<2; k++)
		for (int i = 0; i < 10; ++i)
		{
			image = cv::imread(to_string(k) + "_" + to_string(i+1) +".png" ,cv::IMREAD_GRAYSCALE);
			X->push_back(image_to_vector(image));
			y->push_back(k);
		}
}

int predict(vector<float> X, vector<float> W){
	if(sigmoid(X, W) >= 0.5)
		return 1;
	else
		return 0;
}

// g++ -c main.cpp cso.cpp -std=c++11 `pkg-config --cflags --libs opencv`
// g++  main.o cso.o -o output -std=c++11 `pkg-config --cflags --libs opencv`
// ./output