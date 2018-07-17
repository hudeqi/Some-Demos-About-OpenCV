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
int summ2[10000];
int summ1[10000];


//输入是每个人的目录，每个人的目录里是照片和对应的名称文档txt，都是英文命名，输出是各个名称文件夹，里面是名称的黑底白字二值化切片(汉字距离边界可调)
int main_bhos() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
	Directory dir;
	vector<string> dirNames;
	getAllFiles(dir_path, dirNames);
	fstream outFile;
	int shrink = 0;
	char buffer[256];
	char *end;
	outFile.open("shrink.txt", ios::in);
	outFile.getline(buffer, 256, '\n');
	string s1(buffer);
	shrink = static_cast<int>(strtol(s1.c_str(), &end, 10));
	int cc = 0;
	for (int ii = 0; ii < dirNames.size(); ii++)
	{
		vector<string> fileNames;
		getAllFiles(dirNames[ii], fileNames);
		char Savedir[200];
		strcpy(Savedir, savedir);
		vector<string> compname;
		for (int ii1 = 0; ii1 < fileNames.size(); ii1++) {
			string fileName = fileNames[ii1];
			string ff = fileName.substr(fileName.length() - 1, fileName.length());
			if (ff.compare("g") != 0) {
				string s = "";
				ifstream fin(fileName);
				for (; !fin.eof();) {
					getline(fin, s, '\n');
					compname.push_back(s);
				}
			}
		}
		for (int ii1 = 0; ii1 < fileNames.size(); ii1++) {
			string fileName = fileNames[ii1];
			string ff = fileName.substr(fileName.length() - 1, fileName.length());
			if (ff.compare("g") == 0) {
				cout << "file name:" << fileName << endl;
				Mat img1, roi2;
				Mat img0 = imread(fileName);
				cvtColor(img0, img1, CV_RGB2GRAY);
				threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
				bitwise_not(img1, img1);
				RemoveSmallRegion(img1, img1, 8, 1, 1);
				IplImage *img2 = &IplImage(img1);
				/*uchar *ptr = (uchar *)img2->imageData;
				int width = img2->width;
				int height = img2->height;*/
				//for (int i = 0; i < height; i++) {
				//	for (int j = 0; j < width; j++) {
				//		if (((int)ptr[i*img2->widthStep + j] != 0) && ((int)ptr[i*img2->widthStep + j] != 255))
				//			cout << (int)ptr[i*img2->widthStep + j]<<" ";
				//		/*if ((int)ptr[i*img2->widthStep + j] >= thres)
				//			ptr[i*img2->widthStep + j] = 255;
				//		else
				//		{
				//			ptr[i*img2->widthStep + j] = 0;
				//		}*/
				//	}
				//}
				//cvShowImage("nn", img2);
				//waitKey(0);
				int x1 = getcutpos(img2, 100)[0] - 10;		//整图时，上下左右平切100
				int x2 = getcutpos(img2, 100)[1] + 10;
				int y1 = getcutpos(img2, 100)[2] - 10;
				int y2 = getcutpos(img2, 100)[3] + 10;
				Mat img3(img2);
				Mat img4 = img3(Range(y1, y2 + 1), Range(x1, x2 + 1));
				IplImage *img5 = &IplImage(img4);
				vector<int> fg;
				int pre, next;
				int width1 = img5->width;
				int height1 = img5->height;
				uchar *ptr1 = (uchar *)img5->imageData;
				for (int i = 0; i < width1; i++) {		//有四周
					for (int j = 0; j < height1; j++) {
						summ2[i] += (int)ptr1[j*img5->widthStep + i];
					}
					if (i == 0)
						pre = summ2[i];
					else
					{
						pre = summ2[i - 1];
						next = summ2[i];
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
							/*if (fg.size() > 1) {
								if ((fg[fg.size() - 1] - fg[fg.size() - 2]) < 100)
									fg.pop_back();
							}
							if (fg.size() > 1) {
								if ((fg[fg.size() - 1] - fg[fg.size() - 2]) > 200) {
									int temp1 = fg[fg.size() - 1];
									int temp2 = (fg[fg.size() - 1] + fg[fg.size() - 2]) / 2;
									fg.pop_back();
									fg.push_back(temp2);
									fg.push_back(temp1);
								}
							}*/
						}
					}
				}
				fg.pop_back();
				fg.push_back(width1);
				fg.insert(fg.begin(), 0);

				vector<int> fg1;
				int pre1, next1;
				for (int i = 0; i < height1; i++) {		//有四周
					for (int j = 0; j < width1; j++) {
						summ1[i] += (int)ptr1[i*img5->widthStep + j];
					}
					if (i == 0)
						pre1 = summ1[i];
					else
					{
						pre1 = summ1[i - 1];
						next1 = summ1[i];
					}
					if ((pre1 > 0) && (next1 == 0)) {
						fg1.push_back(i);
					}
					if ((pre1 == 0) && (next1 > 0)) {
						if (fg1.size() != 0)
						{
							fg1.push_back(i - 1);
							int fgpoint = ((fg1[fg1.size() - 1] + fg1[fg1.size() - 2]) / 2);
							fg1.pop_back();
							fg1.pop_back();
							fg1.push_back(fgpoint);
							/*if (fg1.size() > 1) {
								if ((fg1[fg1.size() - 1] - fg1[fg1.size() - 2]) < 100)
									fg1.pop_back();
							}
							if (fg1.size() > 1) {
								if ((fg1[fg1.size() - 1] - fg1[fg1.size() - 2]) > 200) {
									int temp1 = fg1[fg1.size() - 1];
									int temp2 = (fg1[fg1.size() - 1] + fg1[fg1.size() - 2]) / 2;
									fg1.pop_back();
									fg1.push_back(temp2);
									fg1.push_back(temp1);
								}
							}*/
						}
					}
				}
				fg1.pop_back();
				fg1.push_back(height1);
				fg1.insert(fg1.begin(), 0);
				int complength = compname.size();
				for (int num = 0; num < complength - 1; num++) {
					string resultdir1 = compname[num];
					stringstream ss;
					int n = cc * 10 + num + 1;
					string resultdir;
					ss << n;
					ss >> resultdir;
					//string str1 = "分割\\";
					string str1 = "分割\\" + resultdir + "\\";
					char*pdir = (char*)str1.data();
					char Savedirclone[200];
					strcpy(Savedirclone, Savedir);
					strcat(Savedirclone, pdir);
					FILE *fp = NULL;
					fp = fopen(Savedirclone, "w");
					if (!fp)
						mkdir(Savedirclone);
					int len = resultdir1.length();
					Mat *matroi = new Mat[len];
					//Mat *matroi = new Mat[complength];
					char save_file[200];
					//将数字符串转为char并遍历每个字命名照片start
					char *charresultdir = (char*)resultdir.data();
					for (int j = 0; j < len; j++) {
						matroi[j] = img4(Range(fg1[num], fg1[num + 1]), Range(fg[j], fg[j + 1]));
						//matroi[num] = img4(Range(fg1[num], fg1[num + 1]), Range(0, img4.cols));
						sprintf(save_file, "%s%d.png", Savedirclone, j + 1);
						/*sprintf(save_file, "%s%d.jpg", Savedirclone, cc * 10 + num + 1);
						bitwise_not(matroi[num], matroi[num]);
						imwrite(save_file, matroi[num]);*/
						IplImage *m = &IplImage(matroi[j]);
						int x3 = getcutpos(m, 0)[0];
						int x4 = getcutpos(m, 0)[1];
						int y3 = getcutpos(m, 0)[2];
						int y4 = getcutpos(m, 0)[3];
						bitwise_not(matroi[j], matroi[j]);//转为白底黑字
						imwrite(save_file, matroi[j](Range(y3 - shrink, y4 + shrink), Range(x3 - shrink, x4 + shrink)));
					}
				}
			}
		}
		memset(summ2, 0, 10000);
		memset(summ1, 0, 10000);
		cc++;
	}
	//waitKey(0);
	return 0;
}