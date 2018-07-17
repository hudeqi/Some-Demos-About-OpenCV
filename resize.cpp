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
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
	string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));//�����ļ�������
																					   //getAllFiles1(p.assign(path).append("\\").append(fileinfo.name), files);//�ݹ鵱ǰ�ļ���
				}
			}
			else    //�ļ�����
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//�ļ���
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
}

//������ÿ�����ֵ�Ŀ¼��������������Ƭ�������ͬ��Ŀ¼��������resize�����Ƭ
int main_resize() {
	char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //ɾ���ļ�����ֻ���·��
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	string dir_path(savedir);
	dir_path += "���ָ�ͼƬ";
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
		string dir2 = "�ָ�\\" + dirName + "\\";
		//string dir2 = "�ָ�\\";
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