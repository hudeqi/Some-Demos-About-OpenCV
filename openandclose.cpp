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

//图像的开闭操作
void imageOpenClose() {
	const char* name = "C:\\Users\\Administrator\\Desktop\\cc1.jpg";
	IplImage* img = cvLoadImage(name, CV_LOAD_IMAGE_GRAYSCALE);
	if (img == NULL) {
		printf("image load failed.\n");
		return;
	}
	int iterationTimes = 1;//迭代 次数

	IplImage* temp = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

	IplImage* imgDst1 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	//第三个参数是和原图像同样大小的，用于某些操作的中间转换 开闭操作可为空
	//第四个参数是卷积核，默认是3*3
	IplConvKernel *element = cvCreateStructuringElementEx(2, 2, 0.5, 0.5, CV_SHAPE_RECT, 0);
	cvMorphologyEx(img, imgDst1, temp, element, CV_MOP_OPEN, iterationTimes);//
	Mat mm = Mat(imgDst1);
	Mat nn = inhance(mm, 1.5);
	threshold(nn, nn, 0, 255, CV_THRESH_OTSU);
	bitwise_not(nn, nn);
	RemoveSmallRegion(nn, nn, 1, 1, 1);
	bitwise_not(nn, nn);
	imshow("v", nn);
	imwrite("C:\\Users\\Administrator\\Desktop\\bb.jpg", nn);
																		  //对于开操作再进行膨胀，可以填补断裂、空隙
	IplImage* imgDst1Better = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvDilate(imgDst1, imgDst1Better, NULL, iterationTimes);

	IplImage* imgDst2 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvMorphologyEx(img, imgDst2, NULL, NULL, CV_MOP_CLOSE, iterationTimes);

	cvNamedWindow("Source", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("open", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("openAndDilate", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("close", CV_WINDOW_AUTOSIZE);

	cvShowImage("Source", img);
	cvShowImage("open", imgDst1);
	cvShowImage("openAndDilate", imgDst1Better);
	cvShowImage("close", imgDst2);


	cvWaitKey();
	cvReleaseImage(&img);
	cvReleaseImage(&imgDst1);
	cvReleaseImage(&imgDst2);
	cvReleaseImage(&imgDst1Better);
	cvDestroyAllWindows();
}

void test1() {
	//Mat src = imread("C:\\Users\\Administrator\\Desktop\\货款.jpg");
	//Mat src = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\形状normalization\\normalization\\TestImgs\\0001 (2).jpg");
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\形状normalization\\normalization\\TestImgs\\45911.png");
	vector<Mat> channels;
	split(src, channels);
	Mat g = channels[2];
	IplImage *gray = &IplImage(g);
	int thres = otsu2(gray);
	cout << thres << endl;
	cvThreshold(gray, gray, thres, 255, CV_THRESH_BINARY);
	cvShowImage("cscs", gray);
	waitKey(0);
}

void test2() {
	//载入原始图    
	Mat image = imread("C:\\Users\\Administrator\\Desktop\\dd.jpg");
	namedWindow("【原始图】膨胀");
	namedWindow("【效果图】膨胀");
	//显示原始图   
	imshow("【原始图】膨胀", image);
	cvtColor(image, image, CV_BGR2GRAY);
	threshold(image, image, 0, 255, CV_THRESH_OTSU);
	bitwise_not(image, image);
	//resize(image, image, Size(image.cols * 2, image.rows * 2), INTER_LANCZOS4);
	imshow("csdc", image);
	//定义核  
	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
	//进行形态学操作  
	morphologyEx(image, image, MORPH_DILATE, element);
	//显示效果图   
	//thinimage(image);
	bitwise_not(image, image);
	imwrite("C:\\Users\\Administrator\\Desktop\\gg.png", image);
	imshow("【效果图】膨胀", image);

	waitKey(0);
}

void gray_level_normlization() {
	//Mat src1 = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\归一化\\形状normalization\\normalization\\TestImgs\\0238001029760000ASYNCA170505112721FO_0001.jpg");
	Mat src1 = imread("C:\\Users\\Administrator\\Desktop\\cc1.jpg");
	//Mat src1 = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\归一化\\形状normalization\\normalization\\TestImgs\\45911.png");
	//Mat src1 = imread("C:\\Users\\Administrator\\Desktop\\数字识别项目\\归一化\\形状normalization\\normalization\\TestImgs\\10ea6472bfc74773a787dc38f660255e8.jpg");
	Mat src;
	//bilateralFilter(src1, src, 40, 80, 20);
	//imshow("cdsyhf", src);
	vector<Mat> channels;
	split(src1, channels);
	Mat gray, mat_mean, mat_stddev;
	//cvtColor(src, gray, CV_RGB2GRAY); // 转换为灰度图
	gray = channels[2];
	imshow("0", gray);
	IplImage *grayIpl = &IplImage(gray);
	Mat grayclone = gray.clone();
	IplImage *graycloneIpl = &IplImage(grayclone);
	int thres = otsu2(graycloneIpl);

	IplImage* img = cvCreateImage(cvGetSize(grayIpl), IPL_DEPTH_8U, 1);
	int width = grayIpl->width;
	int height = grayIpl->height;
	uchar *ptr = (uchar *)grayIpl->imageData;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if ((int)ptr[j*grayIpl->widthStep + i] >= thres) {
				((uchar *)(img->imageData + j*img->widthStep))[i] = 255;
			}
			else
			{
				((uchar *)(img->imageData + j*img->widthStep))[i] = (int)ptr[j*grayIpl->widthStep + i];
			}
		}
	}
	cvShowImage("1", img);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			((uchar *)(img->imageData + j*img->widthStep))[i] = 255 - (int)ptr[j*grayIpl->widthStep + i];
			//cout << (int)ptr[j*grayIpl->widthStep + i] << endl;
		}
	}
	cvShowImage("2", img);
	double m0 = 160;
	double sigma0 = 20;
	Mat Mimg = Mat(img);
	meanStdDev(Mimg, mat_mean, mat_stddev);
	uchar *ptr1 = (uchar *)img->imageData;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int g = (int)ptr1[j*img->widthStep + i];
			if ((g >= 0) && (g <= 254)) {
				double m = mat_mean.at<double>(0, 0);
				double sigma = mat_stddev.at<double>(0, 0);
				if ((g == 0) || (g == m) || (g == (m + 2 * sigma))) {
					//nonlinear
					double p = log((m0 / (m0 + 2 * sigma0))) / log((m / (m + 2 * sigma)));
					double alpha = m0 / pow(m, p);
					double gg = alpha * (pow(g, p));
					((uchar *)(img->imageData + j*img->widthStep))[i] = 255 - (int)gg;
					/*int t1 = 255 - (int)gg;
					if ((t1 != 0) && (t1 != 255))
						cout << t1 << endl;*/
				}
				//linear
				else
				{
					int gg = (g - m) * (sigma0 / sigma) + m0;
					if (gg > 255)
						gg = 255;
					if (gg < 0)
						gg = 0;
					((uchar *)(img->imageData + j*img->widthStep))[i] = 255 - gg;
					/*int t3 = 255 - (int)gg;
					if ((t3 != 0) && (t3 != 255))
						cout << t3 << endl;*/
				}
			}
			else
			{
				((uchar *)(img->imageData + j*img->widthStep))[i] = 255 - g;
				/*int t2 = 255 - (int)g;
				if ((t2 != 0) && (t2 != 255))
					cout << t2 << endl;*/
			}
		}
	}
	cvShowImage("3", img);
	waitKey(0);
}
//剪背景
void subback() {
	Mat img_src1, img_src2, img_dst, gray1, gray2, gray_diff;
	img_src1 = imread("C:\\Users\\Administrator\\Desktop\\12.jpg");
	//Mat img1 = inhance(img_src1, 3.0);
	cvtColor(img_src1, gray1, CV_BGR2GRAY);
	imshow("video_src", img_src1);
	img_src2 = imread("C:\\Users\\Administrator\\Desktop\\11.jpg");
	//Mat img2 = inhance(img_src2, 3.0);
	cvtColor(img_src2, gray2, CV_BGR2GRAY);
	imshow("video_dst", img_src2);
	subtract(gray1, gray2, gray_diff);
	//for (int i = 0; i<gray_diff.rows; i++)
	//	for (int j = 0; j<gray_diff.cols; j++)
	//		if (abs(gray_diff.at<unsigned char>(i, j)) >= threshold_diff)//这里模板参数一定要用unsigned char，否则就一直报错
	//			gray_diff.at<unsigned char>(i, j) = 255;
	//		else gray_diff.at<unsigned char>(i, j) = 0;
	Mat gg = inhance(gray_diff, 1.0);
	threshold(gray_diff, gray_diff, 0, 255, CV_THRESH_BINARY);
	threshold(gg, gg, 0, 255, CV_THRESH_BINARY);
	imshow("se", gray_diff);
	imshow("se", gray_diff);
	imshow("foreground", gg);
	waitKey(0);

}
int main_nmnma() {
	
	//test1();
	//gray_level_normlization();
	//subback();
	//imageOpenClose();
	test2();

	return 0;
}