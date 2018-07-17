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
#include <io.h>
using namespace std;
using namespace cv;
//#define N 1000
//int center[N];
//int summ[10000];
//int IterationThreshold(CvMat* gray)    //迭代法
//{
//	int width = gray->width;
//	int height = gray->height;
//
//	//直方图统计
//	int histData[256] = { 0 };
//	for (int j = 0; j < height; j++)
//	{
//		uchar*data = (uchar*)(gray->data.ptr + j * gray->step);
//		for (int i = 0; i < width; i++)
//		{
//			histData[data[i]]++;
//		}
//	}
//
//	//求图像的平均灰度值作为初始阈值
//	int T0 = 0;
//	for (int i = 0; i < 256; i++)
//	{
//		T0 += i * histData[i];
//	}
//	T0 /= width * height;
//
//	//迭代
//	int T1 = 0, T2 = 0;
//	int num1 = 0, num2 = 0;
//	int T = 0;
//	while (1)
//	{
//		for (int i = 0; i < T0 + 1; i++)
//		{
//			T1 += i * histData[i];
//			num1 += histData[i];
//		}
//
//		if (num1 == 0)
//			continue;
//
//		for (int i = T0 + 1; i < 256; i++)
//		{
//			T2 += i * histData[i];
//			num2 += histData[i];
//		}
//
//		if (num2 == 0)
//			continue;
//
//		T = (T1 / num1 + T2 / num2) / 2;
//
//		if (T == T0)
//			break;
//		else
//			T0 = T;
//	}
//
//	return T;
//}
////最原始K=2，3切割（现在几乎没用）
//int main(int argc, char** argv) {
//	Mat img, img0, img1, img2;
//	Mat CannyImg;
//	Mat DstImg;
//	IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
//	int i, j, min, max, K;
//	min = 256;
//	max = 0;
//	img = imread("num1.jpg");
//	//显示原图像
//	namedWindow("img", WINDOW_AUTOSIZE);
//	imshow("img", img);
//	//灰度化
//	cvtColor(img, img0, CV_RGB2GRAY);
//	namedWindow("img0", WINDOW_AUTOSIZE);
//	imshow("img0", img0);
//	img1 = img0;
//	//放大二倍
//	//resize(img0,img1,Size(img0.cols*2,img0.rows*2),0,0,INTER_LINEAR);
//	//namedWindow("img1",WINDOW_AUTOSIZE);
//	//imshow("img1",img1);
//	////找min、max
//	//img2 = img1;
//	//for(i = 0;i < img1.rows;i ++){
//	//	for(j = 0;j < img1.cols;j ++){
//	//		if (img1.at<uchar>(i,j) < min)
//	//			min = img1.at<uchar>(i,j);
//	//		if (img1.at<uchar>(i,j) > max)
//	//			max = img1.at<uchar>(i,j);
//	//	}
//	//}
//	//min = min * min;
//	//max = max * max;
//	////增加区分度并归一化
//	//for(i = 0;i < img1.rows;i ++){
//	//	for(j = 0;j < img1.cols;j ++){
//	//		img2.at<uchar>(i,j) = 255*(img1.at<uchar>(i,j) * img1.at<uchar>(i,j)-min)/(max-min); 
//	//	}
//	//}
//	//namedWindow("img2",WINDOW_AUTOSIZE);
//	//imshow("img2",img2);
//	Canny(img0, CannyImg, 140, 250, 3);
//	imshow("CannyImg", CannyImg);
//	cvtColor(img0, DstImg, CV_GRAY2BGR);
//	vector<Vec4i> Lines;
//	HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
//	int *tempy = new int[Lines.size()];
//	for (size_t i = 0; i < Lines.size(); i++)
//	{
//		//line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
//		double a = 0.0;
//		double b = 0.0;
//		a = (Lines[i][3] - Lines[i][1]) / (Lines[i][2] - Lines[i][0]);
//		b = Lines[i][3] - a * Lines[i][2];
//		tempy[i] = a * DstImg.cols / 2 + b;
//	}
//	/*for (i =0; i< Lines.size();i++)
//	cout<<tempy[i]<<endl;*/
//	imshow("HoughLines_Detect", DstImg);
//	imwrite("HoughLines_Detect.jpg", DstImg);
//	//N = Lines.size();
//	//int *center = new int[N];
//	//for (i = 0; i<Lines.size(); i++)
//	//	center[i] = 0;
//	int Num = Lines.size();
//	if (DstImg.rows < 100)
//		K = 2;
//	else
//		K = 3;
//	float *means = new float[K];
//	cluster_p(Num, tempy, K, center, means);
//	/*for (i = 0; i < Lines.size(); i++)
//		cout << tempy[i] << endl;
//	for (i = 0; i < Lines.size(); i++)
//		cout << center[i] << endl;*/
//	int clusters3[3] = { 0,0,0 };
//	int count3[3] = { 0,0,0 };
//	int clusters2[2] = { 0,0 };
//	int count2[2] = { 0,0 };
//	if (K == 3) {
//		for (i = 0; i<Lines.size(); i++) {
//			clusters3[center[i]] += tempy[i];
//			count3[center[i]] += 1;
//		}
//		for (i = 0; i<3; i++) {
//			clusters3[i] /= count3[i];
//			//cout << clusters3[i] << endl;
//		}
//	}
//	if (K == 2) {
//		for (i = 0; i<Lines.size(); i++) {
//			clusters2[center[i]] += tempy[i];
//			count2[center[i]] += 1;
//		}
//		for (i = 0; i<2; i++) {
//			clusters2[i] /= count2[i];
//			//cout << clusters2[i] << endl;
//		}
//	}
//	int minIndex = -1;
//	int maxIndex = -1;
//	if (K == 2)
//	{
//		int indexes[2] = { 0,0 };
//		for (i = 0; i<2; i++)
//		{
//			int tempMaxValue = -1;
//			int tempMaxIndex = -1;
//			for (j = 0; j<2; j++)
//			{
//				if (clusters2[j] > tempMaxValue)
//				{
//					tempMaxValue = clusters2[j];
//					tempMaxIndex = j;
//				}
//			}
//			indexes[i] = tempMaxIndex;
//			clusters2[tempMaxIndex] = 0;
//		}
//		maxIndex = indexes[0];
//		minIndex = indexes[1];
//	}
//	else
//	{
//		int indexes[2] = { 0,0 };
//		for (i = 0; i<2; i++)
//		{
//			int tempMaxValue = -1;
//			int tempMaxIndex = -1;
//			for (j = 0; j<3; j++)
//			{
//				if (clusters3[j] > tempMaxValue)
//				{
//					tempMaxValue = clusters3[j];
//					tempMaxIndex = j;
//				}
//			}
//			indexes[i] = tempMaxIndex;
//			clusters3[tempMaxIndex] = 0;
//		}
//		maxIndex = indexes[0];
//		minIndex = indexes[1];
//
//	}
//	//cout << maxIndex << " " << minIndex << endl;
//
//	int finalMaxIndex = -1;
//	int finalMinIndex = -1;
//	int finalMaxValue = -1;
//	int finalMinValue = 9999;
//	for (i = 0; i<Lines.size(); i++) {
//		if (center[i] == minIndex)
//		{
//			if (tempy[i]>finalMaxValue)
//			{
//				finalMaxValue = tempy[i];
//				finalMaxIndex = i;
//				/*finalMinValue = tempy[i];
//				finalMinIndex = i;*/
//			}
//		}
//
//		if (center[i] == maxIndex)
//		{
//			if (tempy[i]<finalMinValue)
//			{
//				finalMinValue = tempy[i];
//				finalMinIndex = i;
//				/*finalMaxValue = tempy[i];
//				finalMaxIndex = i;*/
//			}
//		}
//
//	}
//	//cout << finalMaxValue << " " << finalMaxIndex << " " << finalMinValue << " " << finalMinIndex << endl;
//
//	//cout << Lines[finalMaxIndex][1] << Lines[finalMaxIndex][3] << endl;
//	//line(DstImg, Point(0, Lines[finalMaxIndex][1]), Point(DstImg.cols, Lines[finalMaxIndex][3]), Scalar(0, 0, 255), 2, 8);
//	//line(DstImg, Point(0, Lines[finalMinIndex][1]), Point(DstImg.cols, Lines[finalMinIndex][3]), Scalar(0, 0, 255), 2, 8);
//
//	imshow("HoughLines_Detect", DstImg);
//	imwrite("HoughLines_Detect.jpg", DstImg);
//
//	//cout << tempy[finalMinIndex] << " " << tempy[finalMaxIndex] << endl;
//
//	//Mat roi(DstImg, Rect(0, tempy[finalMinIndex]+8, DstImg.cols, tempy[finalMaxIndex] - tempy[finalMinIndex]-8));
//	//Mat roi1 = DstImg(Range(finalMaxValue, finalMinIndex), Range(0, DstImg.cols));
//	Mat roi1 = DstImg(Range(tempy[finalMaxIndex] + 8, tempy[finalMinIndex]), Range(0, DstImg.cols));
//	imshow("roi", roi1);
//
//	Mat cannyroi;
//	Canny(roi1, cannyroi, 230, 250);
//	imshow("CannyImg", cannyroi);
//
//
//	if (K == 3) {
//		Mat grayroi1;
//		cvtColor(roi1, grayroi1, CV_BGR2GRAY);
//		////CvMat *b;
//		//CvMat temp = grayroi1; //转化为CvMat类型，而不是复制数据  
//		////cvCopy(&temp, b);
//		//int thres = IterationThreshold(&temp);
//		//cout << thres << endl;
//		//转换为二值图    
//		Mat binaryroi1;
//		threshold(grayroi1, binaryroi1, 90, 255, CV_THRESH_BINARY);
//		imshow("nn", binaryroi1);
//		//二值图 这里进行了像素反转，因为一般我们用255白色表示前景（物体），用0黑色表示背景    
//		Mat reverseBinaryImage;
//		bitwise_not(binaryroi1, reverseBinaryImage);
//
//		vector <vector<Point>>contours;
//		findContours(reverseBinaryImage,
//			contours,   //轮廓的数组  
//			CV_RETR_TREE,   //获取内外轮廓  
//			CV_CHAIN_APPROX_NONE);  //获取每个轮廓的每个像素  
//									//在白色图像上绘制黑色轮廓  
//		Mat result(reverseBinaryImage.size(), CV_8U, Scalar(0));
//		drawContours(result, contours,
//			-1, //绘制所有轮廓  
//			Scalar(255),  //颜色为黑色  
//			2); //轮廓线的绘制宽度为2  
//
//		//namedWindow("contours");
//		//imshow("contours", result);
//
//		//移除过长或过短的轮廓  
//		int cmin = 10; //最小轮廓长度  
//		int cmax = 1000;    //最大轮廓  
//		vector<vector<Point>>::const_iterator itc = contours.begin();
//		while (itc != contours.end())
//		{
//			if (itc->size() < cmin || itc->size() > cmax)
//				itc = contours.erase(itc);
//			else
//				++itc;
//		}
//
//		//在白色图像上绘制黑色轮廓  
//		Mat result_erase(binaryroi1.size(), CV_8U, Scalar(0));
//		drawContours(result_erase, contours,
//			-1, //绘制所有轮廓  
//			Scalar(255),  //颜色为黑色  
//			2); //轮廓线的绘制宽度为2  
//
//		namedWindow("contours_erase");
//		imshow("contours_erase", result_erase);
//		//imwrite("num2_cr.jpg", result_erase);
//
//		//vector<int> fg;
//		//vector<int> avefg;
//		//int fgmin = 99999;
//		//int pre, next, fgcount = 0;
//		//for (i = 0; i < result_erase.cols; i++) {
//		//	for (j = 0; j < result.rows; j++) {
//		//		summ[i] += result_erase.at<uchar>(j, i);
//		//	}
//		//	if (i == 0)
//		//		pre = summ[i];
//		//	else
//		//	{
//		//		pre = summ[i - 1];
//		//		next = summ[i];
//		//	}
//		//	if ((pre == 0) && (next > 0)) {
//		//		fg.push_back(i - 1);
//		//		fgcount += 1;
//		//	}
//		//	if ((pre > 0) && (next == 0)) {
//		//		fg.push_back(i);
//		//		fgcount += 1;
//		//		avefg.push_back(fg[fgcount - 1] - fg[fgcount - 2]);
//		//		if ((fg[fgcount - 1] - fg[fgcount - 2]) < fgmin)
//		//			fgmin = fg[fgcount - 1] - fg[fgcount - 2];
//		//	}	
//		//}
//		//for (i = 0; i < avefg.size(); i++) {
//		//	if (avefg[i] <= 10) {
//		//		fg.erase(fg.begin() + (i * 2));
//		//		fg.erase(fg.begin() + (i * 2));
//		//	}
//		//}
//		//for (i = 0; i < avefg.size(); i++) {
//		//	if ((avefg[i] / fgmin) > 1) {
//		//		int t = avefg[i] / fgmin;
//		//		int s = fg[i * 2];
//		//		for (j = 0; j < t; j++) {
//		//			fg.push_back(s);
//		//			s += (avefg[i] / t);
//		//			fg.push_back(s);
//		//			s++;
//		//		}
//		//		fg.erase(fg.begin() + (i * 2));
//		//		fg.erase(fg.begin() + (i * 2));
//		//	}
//		//	
//		//}
//		//for (i = 0; i < avefg.size(); i++)
//		//	cout << avefg[i] << endl;
//		//cout << endl;
//		//for (i = 0; i < fg.size(); i++)
//		//	cout << fg[i] << endl;
//		//Mat *matroi = new Mat[(fg.size()/ 2)];
//		//for (i = 0; i < fg.size();i += 2)
//		//	matroi[i/2] = result_erase(Range(0, result_erase.rows), Range(fg[i], fg[i+1] + 1));
//		////for (i = 0; i < (fg.size() / 2);i++)
//		//	imshow("num1_fg8", matroi[0]);
//		//	imshow("num1_fg1", matroi[1]);
//		//	imshow("num1_fg2", matroi[2]);
//		//	imshow("num1_fg3", matroi[3]);
//		//	imshow("num1_fg4", matroi[4]);
//		//	imshow("num1_fg5", matroi[5]);
//		//	imshow("num1_fg6", matroi[6]);
//		//	imshow("num1_fg7", matroi[7]);
//		//	imshow("num1_fg9", matroi[8]);
//			//imshow("num1_fg10", matroi[9]);
//	}
//
//
//
//	//imwrite("roi5.jpg", roi);
//	//Mat m2 = roi.clone();
//	//Mat matg = guidedFilter(roi,m2,4,0.01);//添加引导滤波
//	//imshow("guidedfilter",matg);
//	if (K == 2) {
//		IplImage imagesource(roi1);
//		//IplImage *imagesource = cvLoadImage("roi.jpg", 1);
//		img_gray = cvCreateImage(cvGetSize(&imagesource), IPL_DEPTH_8U, 1);
//		//cvShowImage("iG", img_gray);
//		img_equaliz = cvCreateImage(cvGetSize(&imagesource), IPL_DEPTH_8U, 1);
//		//cvShowImage("ie", img_equaliz);
//		img_thread = cvCreateImage(cvGetSize(&imagesource), IPL_DEPTH_8U, 1);
//		//cvShowImage("it", img_thread);
//		img_erode_dilate = cvCreateImage(cvGetSize(&imagesource), IPL_DEPTH_8U, 1);
//		//cvShowImage("ied", img_erode_dilate);
//		cvSmooth(&imagesource, &imagesource, CV_GAUSSIAN, 5, 0);
//		//cvShowImage("imgs", &imagesource);
//		cvCvtColor(&imagesource, img_gray, CV_BGR2GRAY);
//		//cvShowImage("imgg", img_gray);
//		cvEqualizeHist(img_gray, img_equaliz);
//		//cvShowImage("imge", img_equaliz);
//		/*CvMat temp;
//		cvGetMat(img_equaliz, &temp);
//		int thres = IterationThreshold(&temp);
//		cout << thres << endl;*/
//		cvThreshold(img_equaliz, img_thread, 30, 255, CV_THRESH_BINARY_INV);//二值化
//		cvNamedWindow("showthreshold", 1);
//		cvShowImage("showthreshold", img_thread);
//	}
//	//Mat mroi(roi, true);
//	/*Mat m1(img_thread, true);
//	imwrite("num5_thread_45.jpg", m1);*/
//	//Mat m2 = m1.clone();
//	//Mat mthread = imread("num1_thread.jpg");
//	
//	////vector<int> sum(mthread.cols, 0);
//	/*imshow("roi_new", roi);
//	IplImage srcroi(roi);
//	cvSaveImage("roi_new.jpg", &srcroi);*/
//
//	/*Mat grayroi;
//	cvtColor(roi, grayroi, CV_GRAY2BGR);*/
//
//	//IplImage *src = cvLoadImage("roi_new.jpg", 0);
//	////IplImage *src = cvLoadImage("DstImg.jpg", 0);
//	////IplImage *src;
//	//
//	////cvtColor(&src1, &src, CV_BGR2GRAY); 
//
//	/*int nChannel = src->nChannels;
//	int width = src->width;
//	int height = src->height;
//	uchar *ptr = (uchar *)src->imageData;
//
//	for (int i = 0; i<width; i++)
//	{
//		for (int j = 0; j<height; j++)
//		{
//			summ[i] = (int)ptr[j*src->width + i];
//		}
//	}
//
//	for (int i = 0; i < width; i++)
//		cout << i<< " "<< summ[i] << endl;*/
//
//	/*
//	for (int i1 = 0; i1 < DstImg.cols; i1++) {
//		for (int j1 = 0; j1 < DstImg.rows; j1++)
//			summ[i1] += DstImg.at<int>(i1,j1);
//	}
//	for (int i2 = 0; i2 < DstImg.cols; i2++)
//		cout << summ[i2] << endl;
//    */
//
//	/*ofstream ofout("myfile.txt");
//	ofout.open("myfile.txt");
//	for (i = 0; i < mthread.cols; i++)
//		ofout << sum[i] << endl;
//	ofout.close;*/
//	/*FILE *f;
//	f = fopen("myfile.txt", "wt");
//	for (i = 0; i<mthread.cols; i++)
//		fprintf(f, "%d ", sum[i]);*/
//	//fclose(f);
//	//cvErode(img_thread, img_erode_dilate, NULL, 1);//腐蚀
//	//cvNamedWindow("showerode", 1);
//	//cvShowImage("showerode", img_erode_dilate);
//	//IplConvKernel *conv = cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);//膨胀
//	//cvDilate(img_erode_dilate, img_erode_dilate, conv, 1);
//	//cvNamedWindow("showdilate", 1);
//	//cvShowImage("showdilate", img_erode_dilate);
//	/*Mat matg = guidedFilter(m1,m2,4,0.01);//添加引导滤波
//	imshow("guidedfilter",matg);*/
//	/*Mat global;//全局二值化
//	threshold(roi, global, 80, 255, CV_THRESH_BINARY_INV);
//	imshow("globalThreshold", global);*/
//
//
//
//	waitKey(0);
//	return 0;
//}

int main_vfbgn(){
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
	Directory dir;
	vector<string> fileNames;
	getAllFiles(dir_path, fileNames);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		strcpy(Savedir, savedir);
		string fileName = fileNames[ii];
		string ff = fileName.substr(fileName.length() - 4, fileName.length());
		if (ff.compare(".png") == 0) {
			cout << "file name:" << fileName << endl;
			/*Mat img0 = imread(fileName);  //横向分开自己打印的4张支票
			int r = img0.rows / 4;
			Mat img1 = img0(Range(0, r), Range(0, img0.cols - 520));
			Mat img2 = img0(Range(r + 1, 2 * r), Range(0, img0.cols - 520));
			Mat img3 = img0(Range(2 * r + 1, 3 * r), Range(0, img0.cols - 520));
			Mat img4 = img0(Range(3 * r + 1, img0.rows), Range(0, img0.cols - 520));
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\" + nn + "_1.png", img1);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\" + nn + "_2.png", img2);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\" + nn + "_3.png", img3);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\" + nn + "_4.png", img4);*/
		
		
		
			Mat img0 = imread(fileName);		//切自己打印支票的中间
			Mat red = splitTored(img0);
			vector<Vec4i> Lines = drawLinesforPic(red, 500);
			int *tempy = new int[Lines.size()];
			for (int i = 0; i < Lines.size(); i++)
			{
				tempy[i] = Lines[i][3];
			}
			quickSort(tempy, 0, Lines.size() - 1);
			line(red, Point(0, tempy[0]), Point(red.cols - 1, tempy[0]), Scalar(255, 0, 0), 2, 8);
			Mat middle1 = img0(Range(tempy[0] - 15, tempy[0] + 125), Range(120, 1330));
			Mat middle2 = img0(Range(tempy[0] - 15, tempy[0] + 125), Range(1330, img0.cols));
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\" 
				+ nn + "_left.png", middle1);
			imwrite("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.2.8\\nbOCR_2018.2.8\\nbOCR\\Debug\\分割\\"
				+ nn + "_right.png", middle2);
		}
	}
	return 0;
}