#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
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

int main_lcl() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "´ý·Ö¸îÍ¼Æ¬";
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
			int *range = new int[2];
			int cp = Mapsumtocut_with_range(img, range);
			Mat mat1 = img(Range(0, img.rows), Range(0, range[0]));
			Mat mat2 = img(Range(0, img.rows), Range(range[1], img.cols));
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "·Ö¸î\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			IplImage *a1ipl1 = &IplImage(mat1);
			int x11 = getcutpos(a1ipl1, 0)[0];
			int x12 = getcutpos(a1ipl1, 0)[1];
			int y11 = getcutpos(a1ipl1, 0)[2];
			int y12 = getcutpos(a1ipl1, 0)[3];
			Mat a1res1 = mat1(Range(y11, y12), Range(x11, x12));
			bitwise_not(a1res1, a1res1);
			char save_file[200];
			int numpic1 = 0;
			sprintf(save_file, "%s%d.png", Savedir, numpic1);
			imwrite(save_file, a1res1);

			IplImage *a1ipl2 = &IplImage(mat2);
			int x21 = getcutpos(a1ipl2, 0)[0];
			int x22 = getcutpos(a1ipl2, 0)[1];
			int y21 = getcutpos(a1ipl2, 0)[2];
			int y22 = getcutpos(a1ipl2, 0)[3];
			Mat a1res2 = mat2(Range(y21, y22), Range(x21, x22));
			bitwise_not(a1res2, a1res2);
			int numpic2 = 1;
			sprintf(save_file, "%s%d.png", Savedir, numpic2);
			imwrite(save_file, a1res2);
		}
	}
	return 0;
}