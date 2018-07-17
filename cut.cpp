//#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp>  
//#include <math.h>  
//#include <string.h>  
//#include <opencv/cv.h>  
//#include <stdio.h>
//#include <iostream>
//#include<fstream>
//#include<iomanip>
//#include "guidedFilter.h"
//#include "KMeans.h"
//#include "CCA.h"
//#include <direct.h>
//#include <windows.h>
//#include <tchar.h>
//#include "opencv2\contrib\contrib.hpp"
//
//using namespace std;
//using namespace cv;
//#define N 1000
//int center[N];
//int summ[10000];
//
//Mat norm(const Mat& src) {
//	// Create and return normalized image:  
//	Mat dst;
//	switch (src.channels()) {
//	case 1:
//		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
//		break;
//	case 3:
//		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
//		break;
//	default:
//		src.copyTo(dst);
//		break;
//	}
//	return dst;
//}
//
//int otsu2(IplImage *image)
//{
//	int w = image->width;
//	int h = image->height;
//
//	unsigned char*np; // 图像指针  
//	unsigned char pixel;
//	int thresholdValue = 1; // 阈值  
//	int ihist[256]; // 图像直方图，256个点  
//
//	int i, j, k; // various counters  
//	int n, n1, n2, gmin, gmax;
//	double m1, m2, sum, csum, fmax, sb;
//
//	// 对直方图置零...  
//	memset(ihist, 0, sizeof(ihist));
//
//	gmin = 255; gmax = 0;
//	// 生成直方图  
//	for (i = 0; i < h; i++)
//	{
//		np = (unsigned char*)(image->imageData + image->widthStep*i);
//		for (j = 0; j < w; j++)
//		{
//			pixel = np[j];
//			ihist[pixel]++;
//			if (pixel > gmax) gmax = pixel;
//			if (pixel < gmin) gmin = pixel;
//		}
//	}
//
//	// set up everything  
//	sum = csum = 0.0;
//	n = 0;
//
//	for (k = 0; k <= 255; k++)
//	{
//		sum += k * ihist[k]; /* x*f(x) 质量矩*/
//		n += ihist[k]; /* f(x) 质量 */
//	}
//
//	if (!n)
//	{
//		// if n has no value, there is problems...  
//		//fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");  
//		thresholdValue = 160;
//		goto L;
//	}
//
//	// do the otsu global thresholding method  
//	fmax = -1.0;
//	n1 = 0;
//	for (k = 0; k <255; k++)
//	{
//		n1 += ihist[k];
//		if (!n1) { continue; }
//		n2 = n - n1;
//		if (n2 == 0) { break; }
//		csum += k *ihist[k];
//		m1 = csum / n1;
//		m2 = (sum - csum) / n2;
//		sb = n1 * n2 *(m1 - m2) * (m1 - m2);
//		/* bbg: note: can be optimized. */
//		if (sb > fmax)
//		{
//			fmax = sb;
//			thresholdValue = k;
//		}
//	}
//
//L:
//	for (i = 0; i < h; i++)
//	{
//		np = (unsigned char*)(image->imageData + image->widthStep*i);
//		for (j = 0; j < w; j++)
//		{
//			if (np[j] >= thresholdValue)
//				np[j] = 255;
//			else np[j] = 0;
//		}
//	}
//
//	//cout<<"The Threshold of this Image in Otsu is:"<<thresholdValue<<endl;  
//	return(thresholdValue);
//}
////北理论文求拐点实现（暂时耽搁）
//int main_cut(int argc, char** argv) {
//	fstream outFile;
//	int up, down = 0;
//	char buffer[256];
//	char *end;
//	outFile.open("nameupdownthres.txt", ios::in);
//	outFile.getline(buffer, 256, '\n');
//	string s1(buffer);
//	up = static_cast<int>(strtol(s1.c_str(), &end, 10));
//	outFile.getline(buffer, 256, '\n');
//	string s2(buffer);
//	down = static_cast<int>(strtol(s2.c_str(), &end, 10));
//
//	char savedir[200];
//	TCHAR szFilePath[MAX_PATH + 1];
//	GetModuleFileName(NULL, szFilePath, MAX_PATH);
//	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
//	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
//	string dir_path(savedir);
//	dir_path += "待分割图片\\";
//	Directory dir;
//	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);
//
//	for (int ii = 0; ii < fileNames.size(); ii++)
//	{
//		char Savedir[200];
//		strcpy(Savedir, savedir);
//		string fileName = fileNames[ii];
//		string fileFullName = dir_path + fileName;
//		cout << "file name:" << fileName << endl;
//		cout << "file paht:" << fileFullName << endl << endl;
//
//		Mat img, img0, img1, img2, X, I;
//		Mat CannyImg;
//		Mat DstImg;
//		IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
//		int i, j, min, max, K;
//		min = 256;
//		max = 0;
//
//		int thres = 0;
//
//		/*outFile.getline(buffer, 256, '\n');
//		string s0(buffer);
//		string sname = s0 + ".jpg";*/
//
//		img = imread(fileFullName);
//		//显示原图像
//		//namedWindow("img", WINDOW_AUTOSIZE);
//		//imshow("img", img);
//		//灰度化
//		cvtColor(img, img0, CV_RGB2GRAY);
//		//namedWindow("img0", WINDOW_AUTOSIZE);
//		//imshow("img0", img0);
//		img1 = img0;
//		img1.convertTo(X, CV_32FC1); //gama校正
//		float gamma = 5.0;
//		pow(X, gamma, I);
//		//imshow("Gamma correction image", norm(I));
//		Canny(norm(I), CannyImg, 140, 250, 3);
//		//imshow("CannyImg", CannyImg);
//		cvtColor(img0, DstImg, CV_GRAY2BGR);
//		vector<Vec4i> Lines;
//		HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
//		int *tempy = new int[Lines.size()];
//		for (size_t i = 0; i < Lines.size(); i++)
//		{
//			//line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
//			double a = 0.0;
//			double b = 0.0;
//			a = (Lines[i][3] - Lines[i][1]) / (Lines[i][2] - Lines[i][0]);
//			b = Lines[i][3] - a * Lines[i][2];
//			tempy[i] = a * DstImg.cols / 2 + b;
//		}
//		//imshow("HoughLines_Detect", DstImg);
//		//waitKey(0);
//		//imwrite("HoughLines_Detect.jpg", DstImg);
//		int Num = Lines.size();
//		int maxy = -1;
//		int miny = 9999;
//		if (Num >= 2) {
//			for (int i = 0; i < Num; i++) {
//				if (tempy[i] > maxy)
//					maxy = tempy[i];
//				if (tempy[i] < miny)
//					miny = tempy[i];
//			}
//		}
//		int diff = maxy - miny;
//		if (Num == 0)
//			K = 0;
//		if (Num == 1)
//			K = 1;
//		if (Num >= 2) {
//			if (diff <= 10)
//				K = 1;
//			else if ((diff > 10) && (diff <= 70))
//				K = 2;
//			else
//				K = 3;
//		}
//		if ((K == 2) || (K == 3)) {
//			float *means = new float[K];
//			cluster_p(Num, tempy, K, center, means);
//		}
//		int clusters3[3] = { 0,0,0 };
//		int count3[3] = { 0,0,0 };
//		int clusters2[2] = { 0,0 };
//		int count2[2] = { 0,0 };
//		if (K == 3) {
//			for (i = 0; i < Lines.size(); i++) {
//				clusters3[center[i]] += tempy[i];
//				count3[center[i]] += 1;
//			}
//			for (i = 0; i < 3; i++) {
//				clusters3[i] /= count3[i];
//			}
//		}
//		if (K == 2) {
//			for (i = 0; i < Lines.size(); i++) {
//				clusters2[center[i]] += tempy[i];
//				count2[center[i]] += 1;
//			}
//			for (i = 0; i < 2; i++) {
//				clusters2[i] /= count2[i];
//			}
//		}
//		int minIndex = -1;
//		int maxIndex = -1;
//		if (K == 2)
//		{
//			int indexes[2] = { 0,0 };
//			for (i = 0; i < 2; i++)
//			{
//				int tempMaxValue = -1;
//				int tempMaxIndex = -1;
//				for (j = 0; j < 2; j++)
//				{
//					if (clusters2[j] > tempMaxValue)
//					{
//						tempMaxValue = clusters2[j];
//						tempMaxIndex = j;
//					}
//				}
//				indexes[i] = tempMaxIndex;
//				clusters2[tempMaxIndex] = 0;
//			}
//			maxIndex = indexes[0];
//			minIndex = indexes[1];
//		}
//		if (K == 3)
//		{
//			int indexes[2] = { 0,0 };
//			for (i = 0; i < 2; i++)
//			{
//				int tempMaxValue = -1;
//				int tempMaxIndex = -1;
//				for (j = 0; j < 3; j++)
//				{
//					if (clusters3[j] > tempMaxValue)
//					{
//						tempMaxValue = clusters3[j];
//						tempMaxIndex = j;
//					}
//				}
//				indexes[i] = tempMaxIndex;
//				clusters3[tempMaxIndex] = 0;
//			}
//			maxIndex = indexes[0];
//			minIndex = indexes[1];
//		}
//
//		int finalMaxIndex = -1;
//		int finalMinIndex = -1;
//		int finalMaxValue = -1;
//		int finalMinValue = 9999;
//		for (i = 0; i < Lines.size(); i++) {
//			if (center[i] == minIndex)
//			{
//				if (tempy[i] > finalMaxValue)
//				{
//					finalMaxValue = tempy[i];
//					finalMaxIndex = i;
//				}
//			}
//
//			if (center[i] == maxIndex)
//			{
//				if (tempy[i] < finalMinValue)
//				{
//					finalMinValue = tempy[i];
//					finalMinIndex = i;
//				}
//			}
//
//		}
//
//		//imshow("HoughLines_Detect", DstImg);
//		//imwrite("HoughLines_Detect.jpg", DstImg);
//
//
//		/*outFile.getline(buffer, 256, '\n');
//		string s3(buffer);
//		thres = static_cast<int>(strtol(s3.c_str(), &end, 10));*/
//		Mat roi1, roi0, roi2;
//		if ((K == 2) || (K == 3)) {
//			roi0 = DstImg(Range(0, tempy[finalMaxIndex] + up), Range(0, DstImg.cols));
//			roi1 = DstImg(Range(tempy[finalMaxIndex] + up, tempy[finalMinIndex] + down), Range(0, DstImg.cols));
//			roi2 = DstImg(Range(tempy[finalMinIndex] + down, DstImg.rows), Range(0, DstImg.cols));
//		}
//		if ((K == 0) || (K == 1))
//		{
//			roi1 = DstImg;
//		}
//
//		Mat grayroi1;
//		cvtColor(roi1, grayroi1, CV_BGR2GRAY);
//		Mat binaryroi1;
//		threshold(grayroi1, binaryroi1, 90, 255, CV_THRESH_BINARY);
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
//				//namedWindow("contours");
//				//imshow("contours", result);
//
//				//移除过长或过短的轮廓  
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
//		imshow("era", result_erase);
//		int csize = contours.size();
//		int **Code = new int*[csize];
//		for (int i = 0; i < csize; i++) {
//			int nr = contours[i].size();
//			Code[i] = new int[nr];
//			int nc = 2;
//			for (int j = 0; j < nr; j++) {
//				int x, y;
//				if (j == 0) {
//					x = contours[i][0].x - contours[i][nr - 1].x;
//					y = contours[i][0].y - contours[i][nr - 1].y;
//				}
//				else
//				{
//					x = contours[i][j].x - contours[i][j - 1].x;
//					y = contours[i][j].y - contours[i][j - 1].y;
//				}
//				if ((x == 1) && (y == 0))
//					Code[i][j] = 0;
//				else if ((x == 1) && (y == 1))
//				{
//					Code[i][j] = 1;
//				}
//				else if ((x == 0) && (y == 1))
//				{
//					Code[i][j] = 2;
//				}
//				else if ((x == -1) && (y == 1))
//				{
//					Code[i][j] = 3;
//				}
//				else if ((x == -1) && (y == 0))
//				{
//					Code[i][j] = 4;
//				}
//				else if ((x == -1) && (y == -1))
//				{
//					Code[i][j] = 5;
//				}
//				else if ((x == 0) && (y == -1))
//				{
//					Code[i][j] = 6;
//				}
//				else if ((x == 1) && (y == -1))
//				{
//					Code[i][j] = 7;
//				}
//			}
//		}
//		int **Cur = new int*[csize];
//		for (int i = 0; i < csize; i++) {
//			int nr = contours[i].size();
//			Cur[i] = new int[nr];
//			for (int j = 0; j < nr; j++) {
//				if (j == 0) {
//					Cur[i][j] = abs(Code[i][j] - Code[i][nr - 1]);
//				}
//				else
//				{
//					Cur[i][j] = abs(Code[i][j] - Code[i][j - 1]);
//				}
//				if (Cur[i][j] > 8)
//					Cur[i][j] = 16 - Cur[i][j];
//			}
//		}
//		int **e = new int*[csize];
//		for (int i = 0; i < csize; i++) {
//			int nr = contours[i].size();
//			e[i] = new int[nr];
//			for (int j = 0; j < nr; j++) {
//				if (j == 0) {
//					e[i][j] = Cur[i][j] * (Cur[i][nr - 1] + Cur[i][j] + Cur[i][j + 1]);
//				}
//				else if (j == (nr - 1))
//				{
//					e[i][j] = Cur[i][j] * (Cur[i][j - 1] + Cur[i][j] + Cur[i][0]);
//				}
//				else if ((j > 0) && (j < (nr -1)))
//				{
//					e[i][j] = Cur[i][j] * (Cur[i][j - 1] + Cur[i][j] + Cur[i][j + 1]);
//				}
//			}
//		}
//		int **newe = new int*[csize];
//		for (int i = 0; i < csize; i++) {
//			int nr = contours[i].size();
//			newe[i] = new int[2 * nr];
//			for (int j = 0; j < nr; j++) {
//				newe[i][j] = e[i][j];
//				newe[i][j + nr] = e[i][j];
//			}
//		}
//		vector<int> *e_start = new vector<int>[csize];
//		vector<int> *e_end = new vector<int>[csize];
//		vector<int> *t = new vector<int>[csize];
//		for (int i = 0; i < csize; i++) {
//			int nr = contours[i].size();
//			int t = 4, num1 = 0, num2 = 0, i1 = 1;
//			while (i1 < (2 * nr - 1)) {
//				if ((newe[i][i1 - 1] < t) && (newe[i][i1] >= t)) {
//					e_start[i].push_back(i1);
//					num1 ++;
//					i1 ++;
//					if (num1 > num2) {
//						for (int j = i1; j < (2 * nr - 1); j++) {
//							if ((newe[i][j] >= t) && (newe[i][j + 1] < t)) {
//								e_end[i].push_back(j);
//								i1 = j;
//								num2++;
//								break;
//							}
//						}
//					}
//				}
//				i1++;
//				if (i1 > (nr - 1))
//					break;
//			}
//			for (int i2 = 0; i2 < num1; i2++) {
//				int *e_region = new int[e_end[i][i2]];
//			}
//		}
//		waitKey(0);
//		return 0;
//	}
//}