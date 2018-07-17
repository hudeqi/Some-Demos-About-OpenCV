#include <opencv2/highgui/highgui.hpp> //OpenCV模块highgui头文件
#include <opencv2/imgproc/imgproc.hpp>//OpenCV图像处理头文件
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#include "guidedFilter.h"
#include "KMeans.h"
using namespace cv;

int main_cooro()
{
	//读入图片
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\new\\38134..jpg");
	bitwise_not(img, img);
	IplImage *imag = &(IplImage(img));
	cvShowImage("【原图】腐蚀操作", imag);
	//进行腐蚀操作
	IplImage *dstimag = cvCreateImage(cvGetSize(imag), IPL_DEPTH_8U, 3);
	cvErode(imag, dstimag, NULL, 1);//腐蚀
	cvNamedWindow("showerode", 1);
	cvShowImage("showerode", dstimag);
	Mat bit = Mat(dstimag);
	bitwise_not(bit, bit);
	imwrite("C:\\Users\\Administrator\\Desktop\\38134.jpg", bit);
	//waitKey(0);
	return 0;
}