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
#include "CCA.h"
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include "opencv2\contrib\contrib.hpp"

using namespace std;
using namespace cv;
//在前景图上平移截取背景大小的图并剪除，暴力平移剪除
int main_gnhfgh() {
	Mat img1 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\before.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\0001 (2).jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat fimg1, fimg2;
	if (img1.cols >= img2.cols) {
		fimg1 = img1(Range(0, img1.rows), Range(0, img2.cols));
		fimg2 = img2;
	}
	else
	{
		fimg2 = img2(Range(0, img2.rows), Range(0, img1.cols));
		fimg1 = img1;
	}
	Mat ha = img2.clone();

	//resize(img2, img2, Size(1338, img2.rows), INTER_LANCZOS4);

	/*Mat gray1, gray2;
	cvtColor(img1, gray1, CV_BGR2GRAY);
	cvtColor(img2, gray2, CV_BGR2GRAY);*/
	int h = fimg1.rows;
	//threshold(fimg1, fimg1, 0, 255, CV_THRESH_OTSU);
	//threshold(fimg2, fimg2, 0, 255, CV_THRESH_OTSU);
	imshow("thres_img1", fimg1);
	imshow("thres_img2", fimg2);
	IplImage *im2 = &IplImage(fimg2);
	IplImage *im1 = &IplImage(fimg1);
	/*IplImage *im11 = cvCreateImage(cvSize(im1->width, im1->height), IPL_DEPTH_8U, 1);
	cvConvert(im1, im11);*/
	CvMat *im1_Mat = cvCreateMat(im1->height, im1->width, CV_32FC1);
	cvConvert(im1, im1_Mat);

	int width = im2->width;
	int height = im2->height;
	uchar *ptr = (uchar*)im2->imageData;
	char save_file[200];
	int i1 = 0;
	for (int i = 200; i < 210; i++) {
		for (int j = -29; j < 31; j++) {
			Mat temp1, temp2;
			if (j <= 0) {
				temp1 = fimg1(Range(0, fimg1.rows), Range(0, fimg1.cols + j));
				temp2 = fimg2(Range(i, i + h), Range(-j, fimg2.cols));
			}
			else
			{
				temp1 = fimg1(Range(0, fimg1.rows), Range(j, fimg1.cols));
				temp2 = fimg2(Range(i, i + h), Range(0, fimg2.cols - j));
			}
			IplImage *tempilp1 = &IplImage(temp1);
			CvMat *tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
			cvConvert(tempilp1, tempMat1);
			IplImage *tempilp2 = &IplImage(temp2);
			CvMat *tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
			cvConvert(tempilp2, tempMat2);
			cvAbsDiff(tempMat2, tempMat1, tempMat2);
			IplImage *result = cvCreateImage(cvSize(tempMat2->width, tempMat2->height), IPL_DEPTH_8U, 1);
			cvConvert(tempMat2, result);
			cvThreshold(result, result, 150, 255, CV_THRESH_BINARY);
			i1++;
			sprintf(save_file, "C:\\Users\\Administrator\\Desktop\\afterall\\%d.jpg", i1);
			cvSaveImage(save_file, result);
		}
		/*cvShowImage("mm", result);
		waitKey(0);*/
	}
	//imwrite("C:\\Users\\Administrator\\Desktop\\before_thres.jpg", img1);
	//imwrite("C:\\Users\\Administrator\\Desktop\\after_thres.jpg", img2);
	//waitKey(0);


	return 0;
}