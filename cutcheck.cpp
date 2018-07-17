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

Mat norm1(const Mat& src) {
	// Create and return normalized image:  
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

vector<Mat> getroicheck(Mat img0, int up, int down) {
	Mat img1, X, I, CannyImg, DstImg, roi1, roi2, roi3;
	vector<Mat> mat;
	int K;
	img1 = img0;
	//img1.convertTo(X, CV_32FC1); //gama校正
	//float gamma = 2.0;
	//pow(X, gamma, I);
	//imshow("Gamma correction image", norm(I));
	//Canny(norm1(I), CannyImg, 140, 250, 3);
	Canny(img0, CannyImg, 140, 250, 3);
	//imshow("CannyImg", CannyImg);
	cvtColor(img0, DstImg, CV_GRAY2BGR);
	vector<Vec4i> Lines;
	HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
	vector<Vec4i>::const_iterator itc = Lines.begin();
	while (itc != Lines.end())
	{
		if (abs((*itc)[0] - (*itc)[2]) < 10)
			itc = Lines.erase(itc);
		else
		{
			++itc;
		}
	}
	int *tempy = new int[Lines.size()];
	for (size_t i = 0; i < Lines.size(); i++)
	{
		line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
		double a = 0.0;
		double b = 0.0;
		a = (Lines[i][3] - Lines[i][1]) / (Lines[i][2] - Lines[i][0]);
		b = Lines[i][3] - a * Lines[i][2];
		tempy[i] = a * DstImg.cols / 2 + b;
	}
	quickSort(tempy, 0, Lines.size() - 1);
	vector<int> finaly;
	int fy1 = 0, fy2 = 0;
	for (int i = 0; i < Lines.size(); i++) {
		if (i == 0)
			finaly.push_back(tempy[i]);
		else
		{
			if (tempy[i] - tempy[i - 1] > 10)
				finaly.push_back(tempy[i]);
		}
	}
	if (finaly.size() == 9) {
		fy1 = finaly[4] - up;
		fy2 = finaly[5] + down;
	}
	else if (finaly.size() == 10)
	{
		if (finaly[0] < 10) {
			fy1 = finaly[5] - up;
			fy2 = finaly[6] + down;
		}
		else
		{
			fy1 = finaly[4] - up;
			fy2 = finaly[6] + down;
		}
	}
	else if (finaly.size() == 11)
	{
		fy1 = finaly[5] - up;
		fy2 = finaly[7] + down;
	}
	if ((finaly[0] < 5) && ((100 * finaly[1] / img0.rows) > 25)) {
		fy1 = finaly[1] - up;
		fy2 = finaly[1] + 40 + down;
	}
	if ((double)(finaly[0] / img0.rows) > 0.25) {
		fy1 = finaly[0] - up;
		fy2 = finaly[0] + 40 + down;
	}
	imshow("HoughLines_Detect", DstImg);
	waitKey(0);
	//imwrite("HoughLines_Detect3.jpg", DstImg);
	roi1 = DstImg(Range(fy1, fy2 + 1), Range(0, DstImg.cols));
	roi2 = DstImg(Range(0, fy1), Range(0, DstImg.cols));
	roi3 = DstImg(Range(fy2 + 1, DstImg.rows), Range(0, DstImg.cols));
	mat.push_back(roi1);
	mat.push_back(roi2);
	mat.push_back(roi3);
	return mat;
}
//切整张支票的目标区域（还未竖直切割,1/0,支票无字）
int main_cc(int argc, char** argv) {
	fstream outFile;
	int up, down,whole = 0;
	char buffer[256];
	char *end;
	outFile.open("nameupdownthres.txt", ios::in);
	outFile.getline(buffer, 256, '\n');
	string s1(buffer);
	up = static_cast<int>(strtol(s1.c_str(), &end, 10));
	outFile.getline(buffer, 256, '\n');
	string s2(buffer);
	down = static_cast<int>(strtol(s2.c_str(), &end, 10));
	outFile.getline(buffer, 256, '\n');
	string s3(buffer);
	whole = static_cast<int>(strtol(s3.c_str(), &end, 10));

	char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	string dir_path(savedir);
	dir_path += "待分割图片\\";
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		strcpy(Savedir, savedir);
		string fileName = fileNames[ii];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl << endl;

		Mat img, img0, roi2;
		IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
		int min, max;
		min = 256;
		max = 0;
		int thres = 0;
		img = imread(fileFullName);
		cvtColor(img, img0, CV_RGB2GRAY);
		roi2 = getroicheck(img0, up, down)[0];
		Mat t = getroicheck(img0, up, down)[1];
		Mat d = getroicheck(img0, up, down)[2];
		//}

		/*Mat LabelImg, graylabelImg;
		icvprCcaBySeedFill(img_thread, LabelImg);
		LabelImg.convertTo(graylabelImg, CV_8UC1);
		imshow("cca", graylabelImg);
		waitKey(0);*/



		//粗分割并存储
		/*vector<int> fg;
		int pre, next;
		int width = img_thread->width;
		int height = img_thread->height;
		uchar *ptr = (uchar *)img_thread->imageData;
		for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
		summ[i] += (int)ptr[j*img_thread->widthStep + i];
		}
		if (i == 0)
		pre = summ[i];
		else
		{
		pre = summ[i - 1];
		next = summ[i];
		}
		if ((pre > 0) && (next == 0)) {
		fg.push_back(i);
		}
		if ((pre == 0) && (next > 0)) {
		if (fg.size() != 0)
		{
		fg.push_back(i - 1);
		int fgpoint = ((fg[fg.size() - 1] + fg[fg.size() - 2]) / 2);
		fg.pop_back();
		fg.pop_back();
		fg.push_back(fgpoint);
		}
		}
		}
		fg.pop_back();
		fg.push_back(width);
		fg.insert(fg.begin(), 0);*/
		char save_file[200];
		char save_file1[200];
		char Savedir1[200];
		strcpy(Savedir1, savedir);
		string filename = fileName.substr(0, fileName.length() - 4);
		string str1 = "temp\\lib\\" + filename + "\\";
		char*pdir1 = (char*)str1.data();
		strcat(Savedir1, pdir1);
		string str = "分割\\";
		char*pdir = (char*)str.data();
		strcat(Savedir, pdir);
		FILE *fp = NULL;
		fp = fopen(Savedir1, "w");
		if (!fp)
			mkdir(Savedir1);
		sprintf(save_file, "%s%s.jpg", Savedir, (char*)filename.data());
		imwrite(save_file, roi2);
		if (whole == 1) {
			sprintf(save_file1, "%s%s_%s.jpg", Savedir1, (char*)filename.data(), "1");
			imwrite(save_file1, t);
			memset(save_file1, 0, sizeof(save_file1));
			sprintf(save_file1, "%s%s_%s.jpg", Savedir1, (char*)filename.data(), "2");
			imwrite(save_file1, d);
		}
	}

	//waitKey(0);
	return 0;
}