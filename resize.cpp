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
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include "opencv2\contrib\contrib.hpp"
#include <io.h>
using namespace std;
using namespace cv;

void getAllFiles3(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;  //很少用的文件信息读取结构
	string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));//保存文件夹名字
																					   //getAllFiles1(p.assign(path).append("\\").append(fileinfo.name), files);//递归当前文件夹
				}
			}
			else    //文件处理
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

//输入是每个汉字的目录，里面是他的照片，输出是同样目录，里面是resize后的照片
int main_resize() {
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
	getAllFiles3(dir_path, dirNames);
	for (int ii = 0; ii < dirNames.size(); ii++)
	{
		vector<string> fileNames;
		getAllFiles3(dirNames[ii], fileNames);
		char *dirNamedir = (char*)dirNames[ii].data();
		char *p = strrchr(dirNamedir, '\\');
		string ps(p);
		string dirName = ps.substr(1, ps.length());
		string dir2 = "分割\\" + dirName + "\\";
		//string dir2 = "分割\\";
		char *S = (char*)dir2.data();
		char Savedir[200];
		char savefile[200];
		strcpy(Savedir, savedir);
		strcat(Savedir, S);
		FILE *fp = NULL;
		fp = fopen(Savedir, "w");
		if (!fp)
			mkdir(Savedir);
		for (int i = 0; i < fileNames.size();i++){
			Mat img = imread(fileNames[i]);
			char *pp = strrchr((char*)fileNames[i].data(), '\\');
			string pps(pp);
			string fname = pps.substr(1, pps.length());
			resize(img, img, Size(1338, 632), INTER_LANCZOS4);
			sprintf(savefile, "%s%s", Savedir, (char*)fname.data());
			imwrite(savefile, img);
		}
	}
	return 0;
}