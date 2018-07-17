#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#include "cutcol.h"
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include "opencv2\contrib\contrib.hpp"
#include <io.h>
using namespace std;
using namespace cv;

int adjx, adjy;

void adj(int j, int x, int y)
{
	switch (j) {
		case 1: 
			adjx = x - 1; adjy = y; break;
		case 2: 
			adjx = x - 1; adjy = y + 1; break;
		case 3: 
			adjx = x;   adjy = y + 1; break;
		case 4: 
			adjx = x + 1; adjy = y + 1; break;
		case 5: 
			adjx = x + 1; adjy = y; break;
		case 6: 
			adjx = x + 1; adjy = y - 1; break;
		case 7: 
			adjx = x;   adjy = y - 1; break;
		case 8: 
			adjx = x - 1; adjy = y - 1; break;
	}
}

Mat comp(int x, int y, Mat I)
{
	imshow("vbgb", I);
	Mat result = Mat(I.rows, I.cols, CV_8U);
	int** b = new int*[1001];; // 标记相应像素点是否被用过
	int** s = new int*[101];     // 保存当前路径
	int** rec = new int*[101];;   // 记录当前最长的路径
	int count, find;
	int i, j, k, l, tx, ty, lmax;
	for (int i = 0; i < 1001; i++) {
		b[i] = new int[1001];
	}
	for (int i = 0; i < 101; i++) {
		s[i] = new int[2];
	}
	for (int i = 0; i < 101; i++) {
		rec[i] = new int[2];
	}

	for (i = 1; i <= 1000; i++)
		for (j = 1; j <= 1000; j++)b[i][j] = 0;
	for (i = 0; i <= 100; i++)
		for (j = 0; j <= 1; j++)rec[i][j] = -1;
	for (i = 0; i <= 100; i++)
		for (j = 0; j <= 1; j++)s[i][j] = -1;
	b[x][y] = 1;
	s[0][0] = x; s[0][1] = y;
	lmax = 0;
	count = 0; //路径条数
	l = 0;     //深度
	adjx = x; adjy = y;

l0: l++;
	j = 0; find = 0; //八个方向，1--8
l1: j++;
	//if (adjx == x && adjy == y && j == 1 && j == 2 && j == 7 && j == 8 && j == 3)goto l1;
	if (j>8)goto l2;
	adj(j, adjx, adjy);
	if ((int)I.at<uchar>(adjx, adjy) != 255)goto l1; // 当前要去的方向没有路径
	else if (b[adjx][adjy])goto l1; // 当前要去的像素点已被用过	
	s[l][0] = adjx;
	s[l][1] = adjy;
	b[adjx][adjy] = 1;
	if (!find) find = 1;
	goto l0;
	//l++; //
l2: l--; //回退
	if (!find)
	{
		//for (i = 1; i <= l; i++)printf("%d,%d", s[l][0], s[l][1]);
		if (l > lmax)
		{
			for (i = 1; i <= l; i++)
			{
				rec[i][0] = s[i][0];
				rec[i][1] = s[i][1];
			}
		}
	}
	if (l>0)
	{
		for (i = 0; i <= 100; i++) {
			if (rec[i][0] > 0) {
				result.at<uchar>(rec[i][0], rec[i][1]) = 0;

			}
			//printf("%d,%d,", rec[i][0], rec[i][1]);
		}
		imshow("vdg", result);
		waitKey();
		adjx = s[l][0]; adjy = s[l][1];
		j = 0;
	//	b[tx][ty] = 1; //?
			goto l1;
	}
	for (i = 0; i <= 100; i++) {
		if (rec[i][0] > 0) {
			result.at<uchar>(rec[i][0], rec[i][1]) = 0;
		
		}
		//printf("%d,%d,", rec[i][0], rec[i][1]);
	}
	imshow("vdg", result);
	waitKey();
	return result;
}

int main_bcnh() {
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\123\\91.png");
	cvtColor(src, src, CV_BGR2GRAY);
	Mat srcclone = src.clone();

	Mat tempsrc = src.clone();					//去掉圈内背景的背景骨架
	threshold(tempsrc, tempsrc, 0, 255, CV_THRESH_OTSU);
	copyMakeBorder(tempsrc, tempsrc, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));
	Mat tc = tempsrc.clone();
	IplImage *img = &IplImage(tempsrc);
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cvDrawContours(img, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);
	Mat s1(img);
	bitwise_not(s1, s1);
	cv::threshold(s1, s1, 128, 1, cv::THRESH_BINARY);
	Mat dst = thinImage(s1, -1);
	Mat dstclone_for_crosspoint = dst.clone();
	filterOver(dstclone_for_crosspoint);
	std::vector<cv::Point> points = getPoints(dstclone_for_crosspoint, 4, 6, 12);
	dst = dst * 255;
	/*for (int i = 0; i < points.size(); i++) {
	line(dst, points[i], Point(points[i].x, 0), Scalar(255));
	}
	imshow("gtd", dst);
	waitKey();*/

	/*vector<cv::Point>::iterator it = points.begin();
	for (; it != points.end(); it++)
	{
	circle(dst, *it, 2, 255, 1);
	}*/
	Mat dstc = dst.clone();
	vector<int*> backcoord;
	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++) {
			if ((int)dst.at<uchar>(i, j) == 255) {
				int* a = new int[2];
				a[0] = i;
				a[1] = j;
				backcoord.push_back(a);
			}
		}
	sort(backcoord.begin(), backcoord.end(), coordinateCmp);
	int tempx = backcoord[0][0];
	int tempy = backcoord[0][1];
	if (tempy < dst.cols / 2) {
		vector<int*> neighbor = getneighbor(dst, tempx, tempy);
		while (neighbor.size() == 2) {
			sort(neighbor.begin(), neighbor.end(), contoursizeCmp);
			tempx = neighbor[1][0];
			tempy = neighbor[1][1];
			neighbor = getneighbor(dst, tempx, tempy);
		}
	}
	if (tempy > dst.cols / 2) {
		vector<int*> neighbor = getneighbor(dst, tempx, tempy);
		while (neighbor.size() == 2) {
			sort(neighbor.begin(), neighbor.end(), contoursizeCmp);
			tempx = neighbor[0][0];
			tempy = neighbor[0][1];
			neighbor = getneighbor(dst, tempx, tempy);
		}
	}
	vector<vector<int*>> resultpath;
	vector<vector<int*>> temppathlist;
	vector<int*> temp;
	int* next;
	vector<pair<int*, vector<int*>>> tolookpoint;
	vector<int*> cutpath;
	int* a = new int[2];
	a[0] = tempx;
	a[1] = tempy;
	cutpath.push_back(a);
	Mat path = comp(a[0], a[1], dstc);
	return 0;
}