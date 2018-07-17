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
//批量图片反转
int main_bit() {
	char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	string dir_path(savedir);
	dir_path += "待分割图片\\";
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.bmp", false);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		strcpy(Savedir, savedir);
		string fileName = fileNames[ii];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl << endl;

		Mat img;
		img = imread(fileFullName);
		cvtColor(img, img, CV_RGB2GRAY);
		bitwise_not(img, img);

		string str = "分割\\" + fileName;
		char*pdir = (char*)str.data();
		strcat(Savedir, pdir);
		imwrite(Savedir, img);
	}
	return 0;
}

int main_svfb() {			//贴四边填方
	string dir_path;
	dir_path += "C:\\Users\\Administrator\\Desktop\\9\\";
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		string fileName = fileNames[ii];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl << endl;

		Mat img;
		img = imread(fileFullName, 0);
		threshold(img, img, 0, 255, CV_THRESH_OTSU);
		Mat img_clone = img.clone();
		IplImage *src = &IplImage(img);
		vector<int> cut_pos = getcutpos(src, 0);
		Mat save = img_clone(Range(cut_pos[2], cut_pos[3]), Range(cut_pos[0], cut_pos[1]));
		int width = save.cols;
		int height = save.rows;
		if (width > height) {
			int gap = width - height;
			int add_pix = gap / 2;
			cv::copyMakeBorder(save, save, add_pix, add_pix, 0, 0, cv::BORDER_CONSTANT, cv::Scalar(0));
		}
		else {
			int gap = height - width;
			int add_pix = gap / 2;
			cv::copyMakeBorder(save, save, 0, 0, add_pix, add_pix, cv::BORDER_CONSTANT, cv::Scalar(0));
		}

		string str = "C:\\Users\\Administrator\\Desktop\\9\\9\\" + fileName;
		char*pdir = (char*)str.data();
		strcpy(Savedir, pdir);
		imwrite(Savedir, save);
	}
	return 0;
}


int main_gbhyn() {
	string dir_path;
	dir_path += "C:\\Users\\Administrator\\Desktop\\LowerNum\\HWDB\\test\\00014\\";
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.png", false);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		string fileName = fileNames[ii];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl << endl;

		Mat img;
		img = imread(fileFullName, 0);
		threshold(img, img, 0, 255, CV_THRESH_OTSU);

		string str = "C:\\Users\\Administrator\\Desktop\\LowerNum\\HWDB\\test\\0\\" + fileName;
		char*pdir = (char*)str.data();
		strcpy(Savedir, pdir);
		imwrite(Savedir, img);
	}
	return 0;
}