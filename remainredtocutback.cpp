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
#include "cutcol.h"
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include "opencv2\contrib\contrib.hpp"

using namespace std;
using namespace cv;

void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type)
{
	int threshold;
	src.copyTo(dst);
	int height = dst.rows;
	int width = dst.cols;
	int k;  //用于循环计数传递到外部
	for (int i = 0; i < height - 1; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < width - 1; j++)
		{
			if (type == 0)
			{
				//行消除
				if (p[j] == 255 && p[j + 1] == 0)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 255;
						}
					}
				}
				//列消除
				if (p[j] == 255 && p[j + width] == 0)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 255;
						}
					}
				}
			}
			else  //type = 1
			{
				//行消除
				if (p[j] == 0 && p[j + 1] == 255)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 0;
						}
					}
				}
				//列消除
				if (p[j] == 0 && p[j + width] == 255)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 0;
						}
					}
				}
			}
		}
	}
}

Mat morered1(Mat img) {
	//Mat imgg = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.28\\nbOCR_2018.1.28\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	IplImage* image = &IplImage(img);
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);
	int w, h;
	vector<int> wh;
	cvCvtColor(image, hsv, CV_BGR2HSV);
	int width = hsv->width;
	int height = hsv->height;
	int flag = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);
			CvScalar s;
			if ((s_hsv.val[0]>= 0) && (s_hsv.val[0] <= 8) &&
			(s_hsv.val[1]>=43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2]>=46) && (s_hsv.val[2] <= 255))
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 255;
				cvSet2D(hsv, i, j, s);
			}
			else
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(hsv, i, j, s);
			}
		}
	}

	Mat ss = Mat(hsv);
	cvtColor(ss, ss, CV_HSV2BGR);
	return ss;
}

Mat morered(Mat img) {
	//Mat imgg = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.28\\nbOCR_2018.1.28\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	IplImage* image = &IplImage(img);
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);
	int w, h;
	vector<int> wh;
	cvCvtColor(image, hsv, CV_BGR2HSV);
	int width = hsv->width;
	int height = hsv->height;
	int flag = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);
			CvScalar s;
			/*if ((s_hsv.val[0]>= 0) && (s_hsv.val[0] <= 8) && 
				(s_hsv.val[1]>=43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2]>=46) && (s_hsv.val[2] <= 255))*/
				if ((s_hsv.val[2] >= 0) && (s_hsv.val[2] <= 255))
			{
				s.val[0] = 180;
				s.val[1] = 30;
				s.val[2] = 255;
				cvSet2D(hsv, i, j, s);
			}
			else
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(hsv, i, j, s);
			}
		}
	}

	Mat ss = Mat(hsv);
	imshow("nhsgb", ss);
	cvtColor(ss, ss, CV_HSV2BGR);
	cvtColor(ss, ss, CV_BGR2GRAY);
	threshold(ss, ss, 0, 255, CV_THRESH_OTSU);
	//deleteLines(ss);
	RemoveSmallRegion(ss, ss, 5, 1, 1);
	//delete_jut(ss, ss, 1, 2, 0);
	bitwise_not(ss, ss);
	imshow("nhs1", ss);
	//imshow("nhs", ss);
	//imwrite("C:\\Users\\Administrator\\Desktop\\vb.jpg", ss);
	waitKey();
	return ss;
}

vector<int> normPoint(CvMat *inputImage) {
	Mat img = Mat(inputImage, true);
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(img, 800);
	int firstline = Lines[1][1];
	IplImage* image = cvCreateImage(cvGetSize(inputImage), 8, 3);
	cvGetImage(inputImage, image);
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);
	int w, h;
	vector<int> wh;
	cvCvtColor(image, hsv, CV_BGR2HSV);
	int width = hsv->width;
	int height = hsv->height;
	int flag = 0;
	for (int i = firstline; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);
			CvScalar s;
			if (!(((s_hsv.val[0]>0) && (s_hsv.val[0] < 8))))
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(hsv, i, j, s);
			}
			else
			{
				if ((cvGet2D(hsv, i, j + 3).val[0] > 0) && (cvGet2D(hsv, i, j + 3).val[0] < 8) && (j < width - 3)) {
					h = i;
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1)
			break;
	}
	flag = 0;
	for (int i = 0; i < width; i++) {
		for (int j = firstline; j < height; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, j, i);
			CvScalar s;
			if (!(((s_hsv.val[0]>0) && (s_hsv.val[0] < 8))))
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(hsv, j, i, s);
			}
			else
			{
				if ((cvGet2D(hsv, j + 3, i).val[0] > 0) && (cvGet2D(hsv, j + 3, i).val[0] < 8) && (j < height - 3)) {
					w = i;
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1)
			break;
	}
	wh.push_back(w);
	wh.push_back(h);
	return wh;
}

vector<int> normPoint1(CvMat *inputImage) {
	Mat img = Mat(inputImage, true);
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(img, 800);
	int *tempy = new int[Lines.size()];
	for (int i = 0; i < Lines.size(); i++)
	{
		tempy[i] = Lines[i][3];
	}
	quickSort(tempy, 0, Lines.size() - 1);
	int firstline = tempy[0];
	Mat remainredmat = morered1(img);
	//imshow("avd", remainredmat);
	//waitKey();
	IplImage* hsv = &IplImage(remainredmat);
	int w1 = 0;
	int w2 = 0;
	int h1 = 0;
	int h2 = 0;
	vector<int> wh;
	cvCvtColor(hsv, hsv, CV_BGR2HSV);
	int width = hsv->width;
	int height = hsv->height;
	int flag = 0;
	for (int i = firstline; i < firstline + 20; i++) {
		for (int j = 0; j < width * 0.5; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);
			CvScalar s;
			if ((s_hsv.val[0] >= 0) && (s_hsv.val[0] <= 15) &&
				(s_hsv.val[1] >= 43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2] >= 46) && (s_hsv.val[2] <= 255))
			{
					h1 = i;
					flag = 1;
					break;
			}
		}
		if (flag == 1)
			break;
	}
	flag = 0;
	for (int i = 0; i < width * 0.5; i++) {
		for (int j = firstline; j < h1 + 20; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, j, i);
			CvScalar s;
			if ((s_hsv.val[0] >= 0) && (s_hsv.val[0] <= 15) &&
				(s_hsv.val[1] >= 43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2] >= 46) && (s_hsv.val[2] <= 255))
			{
					w1 = i;
					flag = 1;
					break;
			}
		}
		if (flag == 1)
			break;
	}
	flag = 0;
	for (int i = firstline; i < firstline + 20; i++) {
		for (int j = width - 1; j > width * 0.5; j--)
		{
			CvScalar s_hsv = cvGet2D(hsv, i, j);
			CvScalar s;
			if ((s_hsv.val[0] >= 0) && (s_hsv.val[0] <= 15) &&
				(s_hsv.val[1] >= 43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2] >= 46) && (s_hsv.val[2] <= 255))
			{
				h2 = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	flag = 0;
	for (int i = width - 1; i > width * 0.5; i--) {
		for (int j = firstline; j < h2 + 20; j++)
		{
			CvScalar s_hsv = cvGet2D(hsv, j, i);
			CvScalar s;
			if ((s_hsv.val[0] >= 0) && (s_hsv.val[0] <= 15) &&
				(s_hsv.val[1] >= 43) && (s_hsv.val[1] <= 255) && (s_hsv.val[2] >= 46) && (s_hsv.val[2] <= 255))
			{
				w2 = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	wh.push_back(w1);
	wh.push_back(w2);
	wh.push_back(h1);
	wh.push_back(h2);
	return wh;
}


void remaintocut() {
	CvMat *inputImage = NULL;
	CvMat *	outputImage = NULL;

	Mat img1 = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.28\\nbOCR_2018.1.28\\nbOCR\\Debug\\对齐减背景\\扫描0001.jpg");
	Mat img2 = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.28\\nbOCR_2018.1.28\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	Mat fimg1, fimg2;
	//使两张初始图片大小裁剪成一致
	if ((img1.rows >= img2.rows) && (img1.cols >= img2.cols)) {
		fimg1 = img1(Range(0, img2.rows), Range(0, img2.cols));
		fimg2 = img2;
	}
	if ((img1.rows >= img2.rows) && (img1.cols < img2.cols)) {
		fimg1 = img1(Range(0, img2.rows), Range(0, img1.cols));
		fimg2 = img2(Range(0, img2.rows), Range(0, img1.cols));
	}
	if ((img1.rows < img2.rows) && (img1.cols < img2.cols)) {
		fimg1 = img1;
		fimg2 = img2(Range(0, img1.rows), Range(0, img1.cols));
	}
	if ((img1.rows < img2.rows) && (img1.cols >= img2.cols)) {
		fimg1 = img1(Range(0, img1.rows), Range(0, img2.cols));
		fimg2 = img2(Range(0, img1.rows), Range(0, img2.cols));
	}

	imwrite("C:\\Users\\Administrator\\Desktop\\1.jpg", fimg1);
	inputImage = cvLoadImageM("C:\\Users\\Administrator\\Desktop\\1.jpg", 1);
	int w1 = 0;
	int w2 = 0;
	int h1 = 0;
	int h2 = 0;
	//求左上角红色位置，即对齐根据位置
	w1 = normPoint(inputImage)[0];
	h1 = normPoint(inputImage)[1];
	imwrite("C:\\Users\\Administrator\\Desktop\\2.jpg", fimg2);
	CvMat *outputImage1 = cvLoadImageM("C:\\Users\\Administrator\\Desktop\\2.jpg", 1);
	w2 = normPoint(outputImage1)[0];
	h2 = normPoint(outputImage1)[1];

	int chaw = w1 - w2;
	int chah = h1 - h2;
	cvtColor(fimg1, fimg1, CV_BGR2GRAY);
	imshow("dc", fimg1);
	cvtColor(fimg2, fimg2, CV_BGR2GRAY);
	imshow("de", fimg2);
	//threshold(fimg1, fimg1, 0, 255, CV_THRESH_OTSU);
	//threshold(fimg2, fimg2, 0, 255, CV_THRESH_OTSU);
	CvMat *tempMat1, *tempMat2;
	//根据红色点对齐并统一裁剪
	if ((chaw >= 0) && (chah >= 0)) {
		Mat tempimg1 = fimg1(Range(chah, fimg1.rows), Range(chaw, fimg1.cols));
		Mat tempimg2 = fimg2(Range(0, fimg2.rows - chah), Range(0, fimg2.cols - chaw));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw >= 0) && (chah < 0)) {
		Mat tempimg1 = fimg1(Range(0, fimg1.rows + chah), Range(chaw, fimg1.cols));
		Mat tempimg2 = fimg2(Range(-chah, fimg2.rows), Range(0, fimg2.cols - chaw));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw < 0) && (chah >= 0)) {
		Mat tempimg1 = fimg1(Range(chah, fimg1.rows), Range(0, fimg1.cols + chaw));
		Mat tempimg2 = fimg2(Range(0, fimg2.rows - chah), Range(-chaw, fimg2.cols));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw < 0) && (chah < 0)) {
		Mat tempimg1 = fimg1(Range(0, fimg1.rows + chah), Range(0, fimg1.cols + chaw));
		Mat tempimg2 = fimg2(Range(-chah, fimg2.rows), Range(-chaw, fimg2.cols));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	//剪背景
	cvAbsDiff(tempMat2, tempMat1, tempMat2);
	IplImage *result = cvCreateImage(cvSize(tempMat2->width, tempMat2->height), IPL_DEPTH_8U, 1);
	cvConvert(tempMat2, result);
	cvShowImage("diff", result);
	//cvSaveImage("C:\\Users\\Administrator\\Desktop\\after_diff.jpg", result);
	cvThreshold(result, result, 0, 255, CV_THRESH_OTSU);
	cvShowImage("thres", result);
	Mat fresult = Mat(result);
	//去除小白点
	RemoveSmallRegion(fresult, fresult, 4, 1, 1);
	imshow("removesmall", fresult);
	//imwrite("C:\\Users\\Administrator\\Desktop\\after_remove.jpg", fresult);
	waitKey(0);
}

void FotsuSabs() {
	Mat img1 = imread("");
	Mat img2 = imread("");
	vector<Mat> channels1;
	vector<Mat> channels2;
	split(img1, channels1);
	split(img2, channels2);
	Mat red1 = channels1[2];
	Mat red2 = channels2[2];
	threshold(red1, red1, 0, 255, CV_THRESH_OTSU);
	threshold(red2, red2, 0, 255, CV_THRESH_OTSU);
	IplImage *ipl1 = &IplImage(red1);
	IplImage *ipl2 = &IplImage(red2);
	CvMat *cm1;
	CvMat *cm2;
	cm1 = cvCreateMat(ipl1->height, ipl1->width, CV_32FC1);
	cm2 = cvCreateMat(ipl2->height, ipl2->width, CV_32FC1);
	cvConvert(ipl1, cm1);
	cvConvert(ipl2, cm2);
}

void searchline() {
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.28\\nbOCR_2018.1.28\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	cvtColor(img, img, CV_BGR2GRAY);
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(img, 800);
	//Lines = drawLinesforPic1(img, 50);
	for (int i = 0; i < Lines.size(); i++)
	{
		line(img, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 1, 1);
	}
	imshow("vfdbg", img);
	waitKey(0);
}

void searchrec() {
	CvMat *inputImage = NULL;
	CvMat *	outputImage = NULL;

	Mat img1 = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\对齐减背景\\未标题-1.jpg");
	Mat img2 = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	Mat fimg1, fimg2;
	//使两张初始图片大小裁剪成一致
	if ((img1.rows >= img2.rows) && (img1.cols >= img2.cols)) {
		fimg1 = img1(Range(0, img2.rows), Range(0, img2.cols));
		fimg2 = img2;
	}
	if ((img1.rows >= img2.rows) && (img1.cols < img2.cols)) {
		fimg1 = img1(Range(0, img2.rows), Range(0, img1.cols));
		fimg2 = img2(Range(0, img2.rows), Range(0, img1.cols));
	}
	if ((img1.rows < img2.rows) && (img1.cols < img2.cols)) {
		fimg1 = img1;
		fimg2 = img2(Range(0, img1.rows), Range(0, img1.cols));
	}
	if ((img1.rows < img2.rows) && (img1.cols >= img2.cols)) {
		fimg1 = img1(Range(0, img1.rows), Range(0, img2.cols));
		fimg2 = img2(Range(0, img1.rows), Range(0, img2.cols));
	}

	imwrite("C:\\Users\\Administrator\\Desktop\\1.jpg", fimg1);
	inputImage = cvLoadImageM("C:\\Users\\Administrator\\Desktop\\1.jpg", 1);
	int w1 = 0;
	int w2 = 0;
	int h1 = 0;
	int h2 = 0;
	//求左上角红色位置，即对齐根据位置
	w1 = normPoint(inputImage)[0];
	h1 = normPoint(inputImage)[1];
	imwrite("C:\\Users\\Administrator\\Desktop\\2.jpg", fimg2);
	CvMat *outputImage1 = cvLoadImageM("C:\\Users\\Administrator\\Desktop\\2.jpg", 1);
	w2 = normPoint(outputImage1)[0];
	h2 = normPoint(outputImage1)[1];

	int chaw = w1 - w2;
	int chah = h1 - h2;
	cvtColor(fimg1, fimg1, CV_BGR2GRAY);
	imshow("dc", fimg1);
	cvtColor(fimg2, fimg2, CV_BGR2GRAY);
	imshow("de", fimg2);
	//threshold(fimg1, fimg1, 0, 255, CV_THRESH_OTSU);
	//threshold(fimg2, fimg2, 0, 255, CV_THRESH_OTSU);
	CvMat *tempMat1, *tempMat2;
	//根据红色点对齐并统一裁剪
	if ((chaw >= 0) && (chah >= 0)) {
		Mat tempimg1 = fimg1(Range(chah, fimg1.rows), Range(chaw, fimg1.cols));
		Mat tempimg2 = fimg2(Range(0, fimg2.rows - chah), Range(0, fimg2.cols - chaw));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw >= 0) && (chah < 0)) {
		Mat tempimg1 = fimg1(Range(0, fimg1.rows + chah), Range(chaw, fimg1.cols));
		Mat tempimg2 = fimg2(Range(-chah, fimg2.rows), Range(0, fimg2.cols - chaw));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw < 0) && (chah >= 0)) {
		Mat tempimg1 = fimg1(Range(chah, fimg1.rows), Range(0, fimg1.cols + chaw));
		Mat tempimg2 = fimg2(Range(0, fimg2.rows - chah), Range(-chaw, fimg2.cols));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	if ((chaw < 0) && (chah < 0)) {
		Mat tempimg1 = fimg1(Range(0, fimg1.rows + chah), Range(0, fimg1.cols + chaw));
		Mat tempimg2 = fimg2(Range(-chah, fimg2.rows), Range(-chaw, fimg2.cols));
		IplImage *tempilp1 = &IplImage(tempimg1);
		tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
		cvConvert(tempilp1, tempMat1);
		IplImage *tempilp2 = &IplImage(tempimg2);
		tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
		cvConvert(tempilp2, tempMat2);
	}
	//剪背景
	cvAbsDiff(tempMat2, tempMat1, tempMat2);
	IplImage *result = cvCreateImage(cvSize(tempMat2->width, tempMat2->height), IPL_DEPTH_8U, 1);
	cvConvert(tempMat2, result);
	cvShowImage("diff", result);
	//cvSaveImage("C:\\Users\\Administrator\\Desktop\\after_diff.jpg", result);
	cvThreshold(result, result, 0, 255, CV_THRESH_OTSU);
	cvShowImage("thres", result);
	Mat fresult = Mat(result);
	//去除小白点
	RemoveSmallRegion(fresult, fresult, 4, 1, 1);
	imshow("removesmall", fresult);
	//imwrite("C:\\Users\\Administrator\\Desktop\\after_remove.jpg", fresult);
	waitKey(0);
}

void cutb() {
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.31\\nbOCR_2018.1.31\\nbOCR\\Debug\\对齐减背景\\0001.jpg");
	Mat imgcl = img.clone();
	Mat red = morered(imgcl);
	//cvtColor(img, img, CV_BGR2GRAY);
	//imshow("bgf", img);
	//cvtColor(red, red, CV_BGR2GRAY);
	//imshow("bgdf", red);
	//IplImage* imgipl = &IplImage(img);
	//IplImage* redipl = &IplImage(red);
	//CvMat *imgcv, *redcv;
	//imgcv = cvCreateMat(imgipl->height, imgipl->width, CV_32FC1);
	//redcv = cvCreateMat(redipl->height, redipl->width, CV_32FC1);
	//cvConvert(imgipl, imgcv);
	//cvConvert(redipl, redcv);
	//cvAbsDiff(imgcv, redcv, imgcv);
	//IplImage *result = cvCreateImage(cvSize(imgcv->width, imgcv->height), IPL_DEPTH_8U, 1);
	//cvConvert(imgcv, result);
	////cvThreshold(result, result, 100, 255, CV_THRESH_BINARY);
	//cvShowImage("diff", result);
	//waitKey();
}

void simulatecutb() {
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\0001.jpg");
	Mat back = img.clone();
	cvtColor(back, back, CV_BGR2GRAY);
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(back, 800);
	int *tempy = new int[Lines.size()];
	for (int i = 0; i < Lines.size(); i++)
	{
		tempy[i] = Lines[i][3];
	}
	quickSort(tempy, 0, Lines.size() - 1);
	int firstline = tempy[0] + 7;
	int secondline = firstline + 75;
	Mat backup = back(Range(0, firstline), Range(0, back.cols));
	Mat backdown = back(Range(secondline, back.rows), Range(0, back.cols));
	Mat backmid = back(Range(firstline, secondline), Range(0, back.cols));
	Mat backmidred = backmid(Range(0, backmid.rows), Range(backmid.cols / 2, backmid.cols / 2 + 100));
	Mat backmidleft = backmid(Range(0, backmid.rows), Range(0, 0.1418 * backmid.cols));
	Mat backmidright = backmid(Range(0, backmid.rows), Range(0.6361 * backmid.cols, backmid.cols));
	Mat backmidredall = backmid(Range(0, backmid.rows), Range(backmidleft.cols, backmid.cols - backmidright.cols));
	int col = backmid.cols - backmidleft.cols - backmidright.cols;
	int numcopy = col / backmidred.cols;
	int remiannum = col % backmidred.cols;
	Mat remiancopy;
	if (remiannum != 0) {
		remiancopy = backmidred(Range(0, backmidred.rows), Range(0, remiannum));
	}
	Mat combinerowred = backmidred;
	for (int i = 0; i < numcopy - 1; i++) {
		hconcat(combinerowred, backmidred, combinerowred);
	}
	if (remiannum != 0) {
		hconcat(combinerowred, remiancopy, combinerowred);
	}
	Mat combinerow = backmidleft;
	hconcat(combinerow, combinerowred, combinerow);
	hconcat(combinerow, backmidright, combinerow);
	Mat simiulateback = backup;
	vconcat(simiulateback, combinerow, simiulateback);
	vconcat(simiulateback, backdown, simiulateback);
	/*imshow("vsasegva", simiulateback);
	waitKey();*/

	cvtColor(img, img, CV_BGR2GRAY);
	/*IplImage *fimg = &IplImage(img);
	IplImage *bimg = &IplImage(simiulateback);*/
	IplImage *fimg = &IplImage(backmidredall);
	IplImage *bimg = &IplImage(combinerowred);
	CvMat *fcv;
	CvMat *bcv;
	fcv = cvCreateMat(fimg->height, fimg->width, CV_32FC1);
	bcv = cvCreateMat(bimg->height, bimg->width, CV_32FC1);
	cvConvert(fimg, fcv);
	cvConvert(bimg, bcv);
	cvAbsDiff(fcv, bcv, fcv);
	IplImage *result = cvCreateImage(cvSize(fcv->width, fcv->height), IPL_DEPTH_8U, 1);
	cvConvert(fcv, result);
	Mat cc = Mat(result);
	cc = 255 - cc;
	Mat ccr = inhance(cc, 1.0);
	threshold(ccr, ccr, 0, 255, CV_THRESH_OTSU);
	imshow("vva", ccr);
	bitwise_not(ccr, ccr);
	RemoveSmallRegion(ccr, ccr, 10, 1, 1);		//去小区域
	//RemoveSmallRegion(ccr, ccr, 10, 0, 1);		//填小洞
	bitwise_not(ccr, ccr);
	imshow("cfsvf", ccr);
	cvtColor(ccr, ccr, CV_GRAY2BGR);
	Mat thinresult = getthinImage(ccr);
	thinresult = thinresult * 255;
	imshow("vva", thinresult);
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(thinresult, thinresult, MORPH_DILATE, element);
	bitwise_not(thinresult, thinresult);
	imshow("vdfv", thinresult);
	waitKey();
}

void simulatecutb1() {
	Mat img = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\相关数据集\\二期自己扫描转账支票\\1-30-word\\img001_2.jpg");
	Mat back = img.clone();
	cvtColor(back, back, CV_BGR2GRAY);
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(back, 800);
	int *tempy = new int[Lines.size()];
	for (int i = 0; i < Lines.size(); i++)
	{
		tempy[i] = Lines[i][3];
	}
	quickSort(tempy, 0, Lines.size() - 1);
	int firstline = tempy[0] + 20;
	int secondline = firstline + 85;
	Mat backup = back(Range(0, firstline), Range(0, back.cols));
	Mat backdown = back(Range(secondline, back.rows), Range(0, back.cols));
	Mat backmid = back(Range(firstline, secondline), Range(0, back.cols));
	/*imshow("vdb", backmid);
	waitKey();*/
	Mat backmidred = backmid(Range(0, backmid.rows), Range(backmid.cols * 0.58, backmid.cols * 0.58 + 100));
	Mat backmidleft = backmid(Range(0, backmid.rows), Range(0, 0.158 * backmid.cols));
	Mat backmidright = backmid(Range(0, backmid.rows), Range(0.652 * backmid.cols, backmid.cols));
	Mat backmidredall = backmid(Range(0, backmid.rows), Range(backmidleft.cols, backmid.cols - backmidright.cols));
	int col = backmid.cols - backmidleft.cols - backmidright.cols;
	int numcopy = col / backmidred.cols;
	int remiannum = col % backmidred.cols;
	Mat remiancopy;
	if (remiannum != 0) {
		remiancopy = backmidred(Range(0, backmidred.rows), Range(0, remiannum));
	}
	Mat combinerowred = backmidred;
	for (int i = 0; i < numcopy - 1; i++) {
		hconcat(combinerowred, backmidred, combinerowred);
	}
	if (remiannum != 0) {
		hconcat(combinerowred, remiancopy, combinerowred);
	}
	Mat combinerow = backmidleft;
	hconcat(combinerow, combinerowred, combinerow);
	hconcat(combinerow, backmidright, combinerow);
	Mat simiulateback = backup;
	vconcat(simiulateback, combinerow, simiulateback);
	vconcat(simiulateback, backdown, simiulateback);
	/*imshow("vsasegva", simiulateback);
	waitKey();*/

	cvtColor(img, img, CV_BGR2GRAY);
	/*IplImage *fimg = &IplImage(img);
	IplImage *bimg = &IplImage(simiulateback);*/
	IplImage *fimg = &IplImage(backmidredall);
	IplImage *bimg = &IplImage(combinerowred);
	CvMat *fcv;
	CvMat *bcv;
	fcv = cvCreateMat(fimg->height, fimg->width, CV_32FC1);
	bcv = cvCreateMat(bimg->height, bimg->width, CV_32FC1);
	cvConvert(fimg, fcv);
	cvConvert(bimg, bcv);
	cvAbsDiff(fcv, bcv, fcv);
	IplImage *result = cvCreateImage(cvSize(fcv->width, fcv->height), IPL_DEPTH_8U, 1);
	cvConvert(fcv, result);
	//cvShowImage("vds", result);
	Mat cc = Mat(result);
	cc = 255 - cc;
	//Mat ccr = inhance(cc, 1.0);
	threshold(cc, cc, 0, 255, CV_THRESH_OTSU);
	bitwise_not(cc, cc);
	RemoveSmallRegion(cc, cc, 10, 1, 1);		//去小区域
	RemoveSmallRegion(cc, cc, 10, 0, 1);		//填小洞
	bitwise_not(cc, cc);
	imshow("cfsvf", cc);
	cvtColor(cc, cc, CV_GRAY2BGR);
	Mat thinresult = getthinImage(cc);
	thinresult = thinresult * 255;
	imshow("vva", thinresult);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(thinresult, thinresult, MORPH_DILATE, element);
	bitwise_not(thinresult, thinresult);
	imshow("vdfv", thinresult);
	//imwrite("C:\\Users\\Administrator\\Desktop\\cc.jpg", cc);
	//cvShowImage("vdfvs", result);
	//cvThreshold(result, result, 60, 255, CV_THRESH_BINARY);
	//cvShowImage("vds", result);
	//Mat thresult = Mat(result);
	//RemoveSmallRegion(thresult, thresult, 1, 1, 1);
	//bitwise_not(thresult, thresult);
	///*cvtColor(thresult, thresult, CV_GRAY2BGR);
	//Mat thinresult = getthinImage(thresult);*/
	//imshow("vsasa", thresult);
	//imwrite("C:\\Users\\Administrator\\Desktop\\bg.jpg", thresult);
	/*Mat vfdv = imread("C:\\Users\\Administrator\\Desktop\\cc1.jpg");
	cvtColor(vfdv, vfdv, CV_BGR2GRAY);
	threshold(vfdv, vfdv, 0, 255, CV_THRESH_OTSU);*/
	//bitwise_not(vfdv, vfdv);
	//RemoveSmallRegion(vfdv, vfdv, 5, 0, 1);
	//imshow("gfnh", vfdv);
	waitKey();
}

//根据红色区域对齐图片，剪背景，二值化。
int main_vd()
{
	//remaintocut();
	//searchline();
	//searchrec();
	//cutb();
	simulatecutb();
	//simulatecutb1();
	return 0;
}
