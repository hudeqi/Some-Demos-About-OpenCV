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
int summ4[10000];
int summ5[10000];


//输入是一堆图片，输出是一堆横条
int main_vdsv() {
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
	int cc = 280;
	for (int ii = 0; ii < dirNames.size(); ii++)
	{
		vector<string> fileNames;
		getAllFiles(dirNames[ii], fileNames);
		char Savedir[200];
		strcpy(Savedir, savedir);
		for (int ii1 = 0; ii1 < fileNames.size(); ii1++) {
			string fileName = fileNames[ii1];
			string ff = fileName.substr(fileName.length() - 1, fileName.length());
			if (ff.compare("g") == 0) {
				cout << "file name:" << fileName << endl;
				Mat img1, roi2;
				Mat img0 = imread(fileName);
				//flip(img0, img0, 0);
				//flip(img0, img0, 1);
				cvtColor(img0, img1, CV_RGB2GRAY);
				threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
				bitwise_not(img1, img1);
				RemoveSmallRegion(img1, img1, 50, 1, 1);
				//copyMakeBorder(img1, img1, 40,40,40,40,BORDER_CONSTANT, Scalar(0));
				IplImage *img2 = &IplImage(img1);
				int x1 = getcutpos(img2, 20)[0] - 5;		//整图时，上下左右平切100
				int x2 = getcutpos(img2, 20)[1] + 5;
				int y1 = getcutpos(img2, 20)[2] - 5;
				int y2 = getcutpos(img2, 20)[3] + 5;
				Mat img3(img2);
				Mat img4 = img3(Range(y1, y2), Range(x1, x2));
				//imwrite("C:\\Users\\Administrator\\Desktop\\kk.png", img4);
				//imshow("bf", img4);
				//waitKey();
				IplImage *img5 = &IplImage(img4);
				vector<int> fg;
				int pre, next;
				int width1 = img5->width;
				int height1 = img5->height;
				uchar *ptr1 = (uchar *)img5->imageData;
				
				vector<int> fg1;
				int pre1, next1;
				for (int i = 0; i < height1; i++) {		
					for (int j = 0; j < width1; j++) {
						summ4[i] += (int)ptr1[i*img5->widthStep + j] / 255;
					}
					if (i == 0)
						pre1 = summ4[i];
					else
					{
						pre1 = summ4[i - 1];
						next1 = summ4[i];
					}
					if ((pre1 > 0) && (next1 == 0)) {
						fg1.push_back(i);
					}
					if ((pre1 == 0) && (next1 > 0)) {
						if (fg1.size() != 0)
						{
							//if (fg1.size() > 1) {
								/*if ((fg1[fg1.size() - 1] - fg1[fg1.size() - 2]) > 180) {
									int temp1 = fg1[fg1.size() - 1];
									int temp2 = (fg1[fg1.size() - 1] + fg1[fg1.size() - 2]) / 2;
									fg1.pop_back();
									fg1.push_back(temp2);
									fg1.push_back(temp1);
								}
								else
								{*/
									fg1.push_back(i - 1);
									int fgpoint = ((fg1[fg1.size() - 1] + fg1[fg1.size() - 2]) / 2);
									fg1.pop_back();
									fg1.pop_back();
									fg1.push_back(fgpoint);
									line(img4, Point(0, fgpoint), Point(img4.cols - 1, fgpoint), Scalar(255), 2, 8);
									/*if (fg1.size() > 1) {
									if ((fg1[fg1.size() - 1] - fg1[fg1.size() - 2]) < 100)
									fg1.pop_back();
									}*/
								//}
							//}
						}
					}
				}
				imwrite("C:\\Users\\Administrator\\Desktop\\dgb.png", img4);
				imshow("ad", img4);
				waitKey();
				fg1.pop_back();
				fg1.push_back(height1);
				fg1.insert(fg1.begin(), 0);
					string str1 = "分割\\";
					char*pdir = (char*)str1.data();
					char Savedirclone[200];
					strcpy(Savedirclone, Savedir);
					strcat(Savedirclone, pdir);
					Mat *matroi = new Mat[fg1.size() - 1];
					char save_file[200];
					for (int j = 0; j < fg1.size() - 1; j++) {
						matroi[j] = img4(Range(fg1[j], fg1[j + 1]), Range(0, img4.cols));
						sprintf(save_file, "%s%d.png", Savedirclone, cc);
						bitwise_not(matroi[j], matroi[j]);
						if (matroi[j].cols != 0) {
							imwrite(save_file, matroi[j]);
							cc++;
						}
					}
					memset(summ4, 0, 10000);
			}
			memset(summ4, 0, 10000);
		}
	}
	//waitKey(0);
	return 0;
}