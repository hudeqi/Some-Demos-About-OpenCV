#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#ifndef KMeans_H
#define KMeans_H

using namespace std;
using namespace cv;
float getDistance(int point1, int point2);
void getMean(int Num, int *tempy, int K, int *center, float *means);
float getE(int Num, int *tempy, int K, int *center, float *means);
void cluster(int Num, int *tempy, int K, int *center, float *means);
int cluster_p(int Num, int *tempy, int K, int *center, float *means);
#endif KMeans_H