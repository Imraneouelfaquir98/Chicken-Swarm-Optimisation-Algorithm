#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "cso.h"

using namespace std;

vector<float> image_to_vector(cv::Mat image);
float         sigmoid        (vector<float> X         , vector<float> W);
float         cost           (vector<vector<float>>  X, vector<float> y, vector<float> W);
void          load_data      (vector<vector<float>> *X_train, vector<vector<float>> *X_test, vector<float> *y_train,vector<float> *y_test);
int           predict(vector<float> X, vector<float> W);
double        accuracy(vector<vector<float>> X_test, vector<float> y_test, vector<float> W);

int main(int argc, char const *argv[])
{
	vector<vector<float>> X_train, X_test;
	vector<float> y_train, y_test;

	load_data(&X_train, &X_test, &y_train, &y_test);

	vector<float> C1 (X_train[0].size(),-20), C2 (X_train[0].size(), 20);

	CSO cso = CSO(
		X_train[0].size(), // D = Dimension
		  30,							 // N 
		   5,							 // G
		  50,							 // Max iteration
		  C1,							 // Cmin
		  C2,							 // Cmax
		&cost							 // Cost function
	);

	cso.fit(X_train, y_train);
	cout<<"\nTesting dataset:"<<endl;
	for (int i = 0; i < (int)X_test.size(); ++i)
		cout<<"  y_true = "
				<<y_test[i]
				<<", y_pred = "
				<<predict(X_test[i], cso.Gbest)
				<<", sigmoid = "
				<<sigmoid(X_test[i], cso.Gbest)<<endl;

	cout<<"\nAccuracy = "
			<<accuracy(X_test, y_test, cso.Gbest)*100
			<<" %\n"
			<<endl;
  return 0;
}

vector<float> image_to_vector(cv::Mat image){
  vector<float> x;
  x.push_back(1);
  for (int i = 0; i < image.rows; ++i)
  	for (int j = 0; j < image.cols; ++j)
  		x.push_back((float)image.at<uchar>(i, j)/255);
  return x;
}

float sigmoid(vector<float> Xi, vector<float> W){
	float val = 0;
	for (int i = 0; i < (int)Xi.size(); ++i)
		val += (float) Xi[i] * W[i];
	return 1/(1 + exp(-val));
}

float cost(vector<vector<float>> X, vector<float> y, vector<float> W){
	float val = 0;
	for(int i=0; i<(int)X.size(); i++) 
		val += pow(y[i] - sigmoid(X[i], W), 2);
	return (float)val/X.size();
}

void load_data(
		vector<vector<float>> *X_train, 
		vector<vector<float>> *X_test, 
		vector<float> *y_train,
		vector<float> *y_test
	){
	cv::Mat image;
	string path = "./PNEUMONIA/";
	for (int k = 0; k<2; k++){
		for (int i = 0; i < 80; ++i){
			image = cv::imread(path+to_string(k)+"_"+to_string(i+1)+".png",cv::IMREAD_GRAYSCALE);
			cv::resize(image, image, cv::Size(100, 100), 0, 0, cv::INTER_AREA);
			if(i%5 == 0){
				X_test->push_back(image_to_vector(image));
				y_test->push_back(k);
			}
			else{
				X_train->push_back(image_to_vector(image));
				y_train->push_back(k);
			}
			// cv::imshow("Image", image);
			// cv::waitKey(0);
		}
		path = "./NORMAL/";
	}
}

int predict(vector<float> X, vector<float> W){
	if(sigmoid(X, W) >= 0.5)
		return 1;
	else
		return 0;
}

double accuracy(vector<vector<float>> X_test, vector<float> y_test, vector<float> W){
	double acc = 0;
	for (int i = 0; i < (int)X_test.size(); ++i)
		acc += (predict(X_test[i], W) == y_test[i])? 1.0 : 0.0;
	return acc/(double)X_test.size();
}

