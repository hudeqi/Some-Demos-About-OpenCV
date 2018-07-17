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

//输入是每个图片的目录，内部存其切片，输出是这些切片的拼接后的图片列表
int main_splice() {
	char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	string dir_path(savedir);
	dir_path += "待分割图片";
	Directory dir;
	vector<string> dirNames;
	getAllFiles(dir_path, dirNames);
	for (int ii = 0; ii < dirNames.size(); ii++)
	{
		vector<string> fileNames;
		getAllFiles(dirNames[ii], fileNames);
		char *dirNamedir = (char*)dirNames[ii].data();
		char *p = strrchr(dirNamedir, '\\');
		string ps(p);
		string dirName = ps.substr(1, ps.length());
		/*string dir2 = "分割\\" + dirName + "\\";*/
		string dir2 = "分割\\";
		char *S = (char*)dir2.data();
		char Savedir[200];
		strcpy(Savedir, savedir);
		strcat(Savedir, S);
		Mat combine;
		for (int ii1 = 0; ii1 < fileNames.size(); ii1++) {
			stringstream ss;
			ss << ii1;
			string fileName = dirNames[ii] + "\\" + ss.str() + ".jpg";
			Mat img0 = imread(fileName);
			if (ii1 == 0)
				combine = img0;
			else
			{
				if (img0.cols != 0)
					hconcat(combine, img0, combine);
			}		
		}
		/*FILE *fp = NULL;
		fp = fopen(Savedir, "w");
		if (!fp)
			mkdir(Savedir);*/
		string savefile = string(Savedir) + dirName + ".jpg";
		imwrite(savefile, combine);
	}
	waitKey(0);
	return 0;
}