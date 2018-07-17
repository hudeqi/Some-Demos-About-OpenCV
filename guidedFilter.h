#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#ifndef guidedFilter_H
#define guidedFilter_H
using namespace std;
using namespace cv;
cv::Mat guidedFilter(cv::Mat I, cv::Mat p, int r, double eps);
#endif guidedFilter_H