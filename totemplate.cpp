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

void Thin(cv::Mat& src, cv::Mat& dst, int intera)
{
	if (src.type() != CV_8UC1)
	{
		printf("ֻ�ܴ����ֵ��Ҷ�ͼ��\n");
		return;
	}
	//��ԭ�ز���ʱ��copy src��dst
	if (dst.data != src.data)
	{
		src.copyTo(dst);
	}

	int i, j, n;
	int width, height;
	width = src.cols - 1;
	//֮���Լ�1���Ƿ��㴦��8���򣬷�ֹԽ��
	height = src.rows - 1;
	int step = src.step;
	int  p2, p3, p4, p5, p6, p7, p8, p9;
	uchar* img;
	bool ifEnd;
	int A1;
	cv::Mat tmpimg;
	//n��ʾ��������
	for (n = 0; n<intera; n++)
	{
		dst.copyTo(tmpimg);
		ifEnd = false;
		img = tmpimg.data;
		for (i = 1; i < height; i++)
		{
			img += step;
			for (j = 1; j<width; j++)
			{
				uchar* p = img + j;
				A1 = 0;
				if (p[0] > 0)
				{
					if (p[-step] == 0 && p[-step + 1]>0) //p2,p3 01ģʽ
					{
						A1++;
					}
					if (p[-step + 1] == 0 && p[1]>0) //p3,p4 01ģʽ
					{
						A1++;
					}
					if (p[1] == 0 && p[step + 1]>0) //p4,p5 01ģʽ
					{
						A1++;
					}
					if (p[step + 1] == 0 && p[step]>0) //p5,p6 01ģʽ
					{
						A1++;
					}
					if (p[step] == 0 && p[step - 1]>0) //p6,p7 01ģʽ
					{
						A1++;
					}
					if (p[step - 1] == 0 && p[-1]>0) //p7,p8 01ģʽ
					{
						A1++;
					}
					if (p[-1] == 0 && p[-step - 1]>0) //p8,p9 01ģʽ
					{
						A1++;
					}
					if (p[-step - 1] == 0 && p[-step]>0) //p9,p2 01ģʽ
					{
						A1++;
					}
					p2 = p[-step]>0 ? 1 : 0;
					p3 = p[-step + 1]>0 ? 1 : 0;
					p4 = p[1]>0 ? 1 : 0;
					p5 = p[step + 1]>0 ? 1 : 0;
					p6 = p[step]>0 ? 1 : 0;
					p7 = p[step - 1]>0 ? 1 : 0;
					p8 = p[-1]>0 ? 1 : 0;
					p9 = p[-step - 1]>0 ? 1 : 0;
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7 && A1 == 1)
					{
						if ((p2 == 0 || p4 == 0 || p6 == 0) && (p4 == 0 || p6 == 0 || p8 == 0)) //p2*p4*p6=0 && p4*p6*p8==0
						{
							dst.at<uchar>(i, j) = 0; //����ɾ�����������õ�ǰ����Ϊ0
							ifEnd = true;
						}
					}
				}
			}
		}

		dst.copyTo(tmpimg);
		img = tmpimg.data;
		for (i = 1; i < height; i++)
		{
			img += step;
			for (j = 1; j<width; j++)
			{
				A1 = 0;
				uchar* p = img + j;
				if (p[0] > 0)
				{
					if (p[-step] == 0 && p[-step + 1]>0) //p2,p3 01ģʽ
					{
						A1++;
					}
					if (p[-step + 1] == 0 && p[1]>0) //p3,p4 01ģʽ
					{
						A1++;
					}
					if (p[1] == 0 && p[step + 1]>0) //p4,p5 01ģʽ
					{
						A1++;
					}
					if (p[step + 1] == 0 && p[step]>0) //p5,p6 01ģʽ
					{
						A1++;
					}
					if (p[step] == 0 && p[step - 1]>0) //p6,p7 01ģʽ
					{
						A1++;
					}
					if (p[step - 1] == 0 && p[-1]>0) //p7,p8 01ģʽ
					{
						A1++;
					}
					if (p[-1] == 0 && p[-step - 1]>0) //p8,p9 01ģʽ
					{
						A1++;
					}
					if (p[-step - 1] == 0 && p[-step]>0) //p9,p2 01ģʽ
					{
						A1++;
					}
					p2 = p[-step]>0 ? 1 : 0;
					p3 = p[-step + 1]>0 ? 1 : 0;
					p4 = p[1]>0 ? 1 : 0;
					p5 = p[step + 1]>0 ? 1 : 0;
					p6 = p[step]>0 ? 1 : 0;
					p7 = p[step - 1]>0 ? 1 : 0;
					p8 = p[-1]>0 ? 1 : 0;
					p9 = p[-step - 1]>0 ? 1 : 0;
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)>1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9)<7 && A1 == 1)
					{
						if ((p2 == 0 || p4 == 0 || p8 == 0) && (p2 == 0 || p6 == 0 || p8 == 0)) //p2*p4*p8=0 && p2*p6*p8==0
						{
							dst.at<uchar>(i, j) = 0; //����ɾ�����������õ�ǰ����Ϊ0
							ifEnd = true;
						}
					}
				}
			}
		}

		//��������ӵ����Ѿ�û�п���ϸ���������ˣ����˳�����
		if (!ifEnd) break;
	}

}

int otsu(Mat image)
{
	int width = image.cols;
	int height = image.rows;
	int x = 0, y = 0;
	int pixelCount[256];
	float pixelPro[256];
	int i, j, pixelSum = width * height, threshold = 0;

	//uchar* data = (uchar*)image.data;

	//��ʼ��  
	for (i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
	for (i = y; i < height; i++)
	{
		for (j = x; j < width; i++)
		{
			pixelCount[(int)image.at<uchar>(i, j)]++;
		}
	}


	//����ÿ������������ͼ���еı���  
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
	}

	//����ostu�㷨,�õ�ǰ���ͱ����ķָ�  
	//�����Ҷȼ�[0,255],������������ĻҶ�ֵ,Ϊ�����ֵ  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

		for (j = 0; j < 256; j++)
		{
			if (j <= i) //��������  
			{
				//��iΪ��ֵ���࣬��һ���ܵĸ���  
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else       //ǰ������  
			{
				//��iΪ��ֵ���࣬�ڶ����ܵĸ���  
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}

		u0 = u0tmp / w0;        //��һ���ƽ���Ҷ�  
		u1 = u1tmp / w1;        //�ڶ����ƽ���Ҷ�  
		u = u0tmp + u1tmp;      //����ͼ���ƽ���Ҷ�  
								//������䷽��  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
		//�ҳ������䷽���Լ���Ӧ����ֵ  
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	//���������ֵ;  
	return threshold;
}

int main_bjhbjh() {
	//Mat src = imread("C:\\Users\\Administrator\\Desktop\\bjbd1.jpg");
	//Mat part1 = src(Range(0, src.rows), Range(0, src.cols * 0.138));
	//Mat part2 = src(Range(0, src.rows * 0.3896), Range(src.cols * 0.6369, src.cols));

	//Mat sample1 = src(Range(src.rows * 0.3896, src.rows), Range(0.6557 * src.cols, 0.6557 * src.cols + 1));
	//Mat combine1 = sample1;
	//for (int i = 0; i < part2.cols - 1; i++) {
	//	hconcat(combine1, sample1, combine1);
	//}
	//Mat sample = src(Range(0, src.rows), Range(0.6227 * src.cols, 0.6227 * src.cols + 1));
	//int chacol = src.cols - part1.cols - part2.cols;
	//int num = chacol / sample.cols;
	//int last = chacol % sample.cols;
	//Mat combine = sample;
	//for (int i = 0; i < num - 1; i++) {
	//	hconcat(combine, sample, combine);
	//}
	////Mat matlast = sample(Range(0, src.rows), Range(0, last));
	////hconcat(combine, matlast, combine);
	//Mat result1 = part1;
	//hconcat(result1, combine, result1);
	//Mat result2 = part2;
	//vconcat(result2, combine1, result2);
	//hconcat(result1, result2, result1);
	////imwrite("C:\\Users\\Administrator\\Desktop\\smg.jpg", result1);

	//Mat tempimg1;
	//cvtColor(src, tempimg1, CV_BGR2GRAY);
	//Mat tempimg2;
	//cvtColor(result1, tempimg2, CV_BGR2GRAY);
	//IplImage *tempilp1 = &IplImage(tempimg1);
	//CvMat *tempMat1 = cvCreateMat(tempilp1->height, tempilp1->width, CV_32FC1);
	//cvConvert(tempilp1, tempMat1);
	//IplImage *tempilp2 = &IplImage(tempimg2);
	//CvMat *tempMat2 = cvCreateMat(tempilp2->height, tempilp2->width, CV_32FC1);
	//cvConvert(tempilp2, tempMat2);
	//cvAbsDiff(tempMat2, tempMat1, tempMat2);
	//IplImage *result = cvCreateImage(cvSize(tempMat2->width, tempMat2->height), IPL_DEPTH_8U, 1);
	//cvConvert(tempMat2, result);
	//cvShowImage("dedarw", result);
	//cvThreshold(result, result, 45, 255, CV_THRESH_BINARY);
	//cvShowImage("dedaw", result);
	//Mat rr(result);
	//Mat deleLinebinaryImg = rr.clone();
	////������ֵ����ĺ������
	//deleteLines(deleLinebinaryImg);
	//Mat dst;
	//medianBlur(deleLinebinaryImg, dst, 3);
	//imshow("diffgt", dst);
	//Mat binaryLineImgCopy = dst.clone();
	//RemoveSmallRegion(binaryLineImgCopy, binaryLineImgCopy, 20, 1, 1);
	//imshow("diff", binaryLineImgCopy);
	//bitwise_not(binaryLineImgCopy, binaryLineImgCopy);
	//imwrite("C:\\Users\\Administrator\\Desktop\\csc.jpg", binaryLineImgCopy);
	//Mat dd;
	//Thin(binaryLineImgCopy, dd, 1);
	//imshow("cscs", dd);

	Mat src = imread("C:\\Users\\Administrator\\Desktop\\29 (2).jpg");
	Mat dst;
	imshow("cds", src);
	bilateralFilter(src, dst, 40, 80, 20);
	//dst = inhance(dst, 2.0);
	Mat red = splitTored(dst);
	imshow("kks", red);
	imshow("m", dst);
	//int thres = otsu2(&IplImage(red.clone()));
	//cvtColor(red, red, CV_BGR2GRAY);
	int thres = otsu(red);
	Mat red_binary;
	threshold(red, red_binary, thres, 255, CV_THRESH_BINARY_INV);
	imshow("v", red_binary);
	Mat deleLinebinaryImg = red_binary.clone();
	//������ֵ����ĺ������
	deleteLines(deleLinebinaryImg);
	Mat binaryLineImgCopy = deleLinebinaryImg.clone();
	imshow("binary", binaryLineImgCopy);
	RemoveSmallRegion(binaryLineImgCopy, binaryLineImgCopy, 4, 1, 1);
	imshow("vu", binaryLineImgCopy);
	waitKey(0);
	return 0;
}