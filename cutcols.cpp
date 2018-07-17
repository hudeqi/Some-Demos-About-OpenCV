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
#define N 1000
#define MAXSIZE 5000
#define _thetah1 1.0
#define _thetah2 1.0
int center[N];
int summ[10000];


//竖直切割（论文细切割+ 中值复切割）
int main_bbnkj(int argc, char** argv) {
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

		Mat img, img0, CannyImg;
		IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
		int min, max;
		min = 256;
		max = 0;
		int thres = 0;
		/*outFile.getline(buffer, 256, '\n');
		string s0(buffer);
		string sname = s0 + ".jpg";*/
		img = imread(fileFullName);
		//cvtColor(img, img0, CV_RGB2GRAY);
		Mat binaryLineImg, grayroi1, grayroi1c;
		cvtColor(img, grayroi1, CV_BGR2GRAY);
		//imshow("HoughLines_Detect", grayroi1);
		//waitKey(0);

		thres = otsu2(&IplImage(grayroi1.clone()));
		threshold(grayroi1, binaryLineImg, thres, 255, CV_THRESH_BINARY_INV);
		//adaptiveThreshold(~grayroi1, binaryLineImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 15, -2);
		//copy一下，对复制品做操作，防止操作改变输入
		//imshow("sourcebinary", binaryLineImg);
		//复制二值化图片进行擦除横线处理
		Mat deleLinebinaryImg = binaryLineImg.clone();
		deleteLines(deleLinebinaryImg);
		Mat binaryLineImgCopy = deleLinebinaryImg.clone();
		imshow("binary", binaryLineImgCopy);
		//waitKey(0);
		//寻找联通区轮廓，contours将存储构成轮廓的点
		vector<vector<Point>> contours;
		findContours(binaryLineImgCopy,
			contours,   //轮廓的数组  
			//CV_RETR_EXTERNAL,
			CV_RETR_TREE,   //获取内外轮廓  
			CV_CHAIN_APPROX_NONE);
		//移除过长或过短的轮廓  
		removeShortContours(contours, 2, 500);
		Mat contoursImg = Mat(binaryLineImg.rows, binaryLineImg.cols, CV_8U);
		drawContours(contoursImg, contours);
		imshow("contoursImg", contoursImg);
		//将联通区转换为坐标表示并按x1升序排序
		vector<int*> coordinates = CCPoints2Coordinates(contours);
		sort(coordinates.begin(), coordinates.end(), coordinateCmp);
		Mat ccaImg = grayroi1.clone();
		for (int i = 0; i < coordinates.size(); i++) {
			rectangle(ccaImg, Point(coordinates.at(i)[0], coordinates.at(i)[2]), Point(coordinates.at(i)[1], coordinates.at(i)[3]), Scalar(0, 0, 0), 0.2);
		}
		//imshow("cca", ccaImg);
		Mat combinedImg = grayroi1.clone();
		vector<int*> combined = combineOverlapComponents(coordinates);
		adjustCombinedComponent(combined);
		for (int i = 0; i < combined.size(); i++) {
			rectangle(combinedImg, Point(combined.at(i)[0], combined.at(i)[2]), Point(combined.at(i)[1], combined.at(i)[3]), Scalar(0, 0, 0), 0.2);
		}
		//imshow("combine", combinedImg);
		//估计行高
		int LH = estimateComponentsMean(combined);
		//潜在粘连模式，返回的指针指向长度为五的数组，前四个位置存储坐标，第五个位置是潜在模式在combined中的index
		vector<int*> potentialPT = detectPotentialPT(combined, LH, _thetah1, _thetah2);
		//探测单笔画区域
		vector<vector<int*>> singleStrokes = detectSingleStroke(binaryLineImgCopy, potentialPT, LH * 0.25, 1);
		Mat singleStrokeImg = grayroi1.clone();
		for (int i = 0; i < singleStrokes.size(); i++) {
			for (int j = 0; j < singleStrokes.at(i).size(); j++) {
				rectangle(singleStrokeImg, Point(singleStrokes.at(i).at(j)[0], singleStrokes.at(i).at(j)[2]),
					Point(singleStrokes.at(i).at(j)[1], singleStrokes.at(i).at(j)[3]), Scalar(0, 0, 0), 0.2);
			}
		}
		//imshow("single_strokes", singleStrokeImg);
		//使用单笔画区域进一步划分
		vector<int*> split = splitBySingleStroke(combined, singleStrokes);
		Mat splitImg = grayroi1.clone();
		for (int i = 0; i < split.size(); i++) {
			rectangle(splitImg, Point(split.at(i)[0], split.at(i)[2]), Point(split.at(i)[1], split.at(i)[3]), Scalar(0, 0, 0), 0.2);
		}
		imshow("split", splitImg);
		waitKey(0);
		Mat fgImg = grayroi1.clone();
		vector<int> fg;
		vector<int> disPoint;//存储预分割点间的距离
		int prefgpoint = 0;
		for (int i = 0; i < (split.size() - 1); i++) {
			if (i == 0)
				prefgpoint = split.at(i)[0];
			int fgpoint = (split.at(i)[1] + split.at(i + 1)[0]) / 2;
			disPoint.push_back(fgpoint - prefgpoint);
			prefgpoint = fgpoint;
			fg.push_back(fgpoint);		//无复分割
		}
		disPoint.push_back(split.at(split.size() - 1)[1] - prefgpoint);

		int dislen = disPoint.size();
		int mid = Middle(disPoint, dislen);
		/*for (int i = 0; i < split.size(); i++) {	//复分割过程
			if (i == 0) {
				fg.push_back(split.at(i)[0]);
				prefgpoint = split.at(i)[0];
			}
			int fgpoint;
			if (i < (split.size() - 1)) {
				fgpoint = (split.at(i)[1] + split.at(i + 1)[0]) / 2;
			}
			else {
				fgpoint = split.at(i)[1];
			}
			int numfgPoint = disPoint[i] / mid;
			for (int j = 1; j < (numfgPoint + 1); j++) {
				fg.push_back(prefgpoint + (mid * j));
			}
			if ((prefgpoint + (mid * numfgPoint)) != fgpoint)
				fg.push_back(fgpoint);
			prefgpoint = fg[fg.size() - 1];
		}*/

		fg.insert(fg.begin(), 0);
		fg.insert(fg.end(), fgImg.cols);
		Mat *fgroi = new Mat[fg.size() - 1];
		char save_file[200];
		string str = "分割\\" + fileName.substr(0, fileName.length() - 4) + "\\";
		char *p = (char*)str.data();
		strcat(Savedir, p);
		FILE *fp = NULL;
		fp = fopen(Savedir, "w");
		if (!fp)
			mkdir(Savedir);
		for (int i = 0; i < (fg.size() - 1); i ++) {
			fgroi[i] = fgImg(Range(0, fgImg.rows), Range(fg[i], fg[i + 1]));
			sprintf(save_file, "%s%d.jpg", Savedir, i);
			//imwrite(save_file, matroi[j]);
			imwrite(save_file, fgroi[i]);
		}
		//}

		/*Mat LabelImg, graylabelImg;
		icvprCcaBySeedFill(img_thread, LabelImg);
		LabelImg.convertTo(graylabelImg, CV_8UC1);
		imshow("cca", graylabelImg);
		waitKey(0);*/

	}

	waitKey(0);
	return 0;
}