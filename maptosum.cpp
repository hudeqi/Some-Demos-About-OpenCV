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

int main_bnndd() {
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
			Mat img = imread(fileName);
			cvtColor(img, img, CV_BGR2GRAY);
			threshold(img, img, 0, 255, CV_THRESH_OTSU);
			//bitwise_not(img, img);
			int col = img.cols;
			int row = img.rows;
			int *sum = new int[col];			//x轴投影
			for (int i = 0; i < col; i++) {
				int ss = 0;
				for (int j = 0; j < row; j++) {
					ss += ((int)img.at<uchar>(j, i) / 255);
				}
				sum[i] = ss;
			}
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);;
			char save_file[200];
			int numpic = 0;
			sprintf(save_file, "%s%d.txt", Savedir, numpic);
			ofstream outf;
			outf.open(save_file);
			for (int i = 0; i < col; i++) {
				outf << sum[i] << endl;
			}

			outf.close();
		}
	}


	//Mat img = imread("C:\\Users\\Administrator\\Desktop\\0.4.6\\20\\15.jpg");
	//cvtColor(img, img, CV_BGR2GRAY);
	//threshold(img, img, 0, 255, CV_THRESH_OTSU);
	//bitwise_not(img, img);
	//int col = img.cols;
	//int row = img.rows;
	//int *sum = new int[col];			//x轴投影
	//for (int i = 0; i < col; i++) {
	//	int ss = 0;
	//	for (int j = 0; j < row; j++) {
	//		ss += ((int)img.at<uchar>(j, i) / 255);
	//	}
	//	sum[i] = ss;
	//}
	//ofstream outf;
	//outf.open("C:\\Users\\Administrator\\Desktop\\峰值\\9.txt");
	//for (int i = 0; i < col; i++) {
	//	outf << sum[i] << endl;
	//}

	/*int *sum1 = new int[row];
	for (int i = 0; i < row; i++) {
		int ss = 0;
		for (int j = 0; j < col; j++) {
			ss += ((int)img.at<uchar>(i, j) / 255);
		}
		sum1[i] = ss;
	}
	ofstream outf;
	outf.open("C:\\Users\\Administrator\\Desktop\\0.4.6\\20\\15.txt");
	for (int i = 0; i < row; i++) {
		outf << sum1[i] << endl;
	}*/
	//outf.close();
	return 0;
}