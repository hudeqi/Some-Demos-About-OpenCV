#include <opencv2/highgui/highgui.hpp> //OpenCVģ��highguiͷ�ļ�
#include <opencv2/imgproc/imgproc.hpp>//OpenCVͼ����ͷ�ļ�
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
	//����ͼƬ
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\new\\38134..jpg");
	bitwise_not(img, img);
	IplImage *imag = &(IplImage(img));
	cvShowImage("��ԭͼ����ʴ����", imag);
	//���и�ʴ����
	IplImage *dstimag = cvCreateImage(cvGetSize(imag), IPL_DEPTH_8U, 3);
	cvErode(imag, dstimag, NULL, 1);//��ʴ
	cvNamedWindow("showerode", 1);
	cvShowImage("showerode", dstimag);
	Mat bit = Mat(dstimag);
	bitwise_not(bit, bit);
	imwrite("C:\\Users\\Administrator\\Desktop\\38134.jpg", bit);
	//waitKey(0);
	return 0;
}