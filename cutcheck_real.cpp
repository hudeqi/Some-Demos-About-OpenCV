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

vector<Mat> getroicheck2(Mat img0, int up, int down, Mat img) {
	Mat img1, X, I, CannyImg, DstImg, roi1, roi2, roi3, roi4, roi5, roi6;
	vector<Mat> mat;
	int K;
	//img0 = inhance(img0, 2.0);
	//imshow("CannyImg", CannyImg);
	cvtColor(img0, DstImg, CV_GRAY2BGR);
	bool out = false;
	vector<Vec4i> Lines;
	Lines = drawLinesforPic(img0, 400);

	again:
	int *tempy = new int[Lines.size()];
	for (int i = 0; i < Lines.size(); i++)
	{
		//line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
		tempy[i] = Lines[i][3];
	}
	//imshow("HoughLines_Detect", DstImg);
	//waitKey(0);
	quickSort(tempy, 0, Lines.size() - 1);
	int count = 0;
	/*for (int i = 0; i < Lines.size(); i++) {
		cout << tempy[i] << endl;
	}*/
	for (int i = 0; i < Lines.size(); i++) {
		if ((tempy[i] > 206) && (tempy[i] < 277))
			count++;
	}
	if (count > 20) {
		Lines.clear();
		delete[]tempy;
		Lines = drawLinesforPic(img0, 800);
		out = true;
		goto again;
	}
	int fy1 = 0, fy2 = 0;
	if (out) {
		int upedge = 0;
		bool flag = true; 
		int count = 0;
		for (int i = 0; i < Lines.size(); i++) {
			if ((tempy[i] > (0.3274 * DstImg.rows - 7)) && (tempy[i] < (0.3274 * DstImg.rows + 7))) {
				upedge = tempy[i];
				count++;
				break;
			}
		}
		if (count == 0)
			flag = false;
		if (flag) {
			fy1 = upedge;
		}
		else
		{
			fy1 = DstImg.rows * 0.3274;
		}
		fy2 = fy1 + 72;
		roi2 = img(Range(fy2 - up, fy2 + 62 + 1 + down), Range(0, DstImg.cols));
		roi1 = img(Range(fy1 - up, fy2 + 1 + down), Range(0, DstImg.cols));
		roi3 = img(Range(fy1 - 45 - up, fy1 + 1 + down), Range(0, DstImg.cols));
		roi4 = img(Range(fy1 - 84 - up, fy1 - 44 + 1 + down), Range(0, DstImg.cols));
		roi5 = img(Range(DstImg.rows - 100 - up, DstImg.rows), Range(0, DstImg.cols));
		roi6 = img(Range(fy2 - up, fy2 + 250 + 1 + down), Range(0, DstImg.cols));
		mat.push_back(roi1);
		mat.push_back(roi2);
		mat.push_back(roi3);
		mat.push_back(roi4);
		mat.push_back(roi5);
		mat.push_back(roi6);
	}
	else
	{
		vector<Vec4i> lines;
		lines = drawLinesforPic(img0, 100);
		int count_1 = 0;
		for (int i = 0; i < lines.size(); i++) {
			if ((lines[i][1] > 206) && (lines[i][1] < 277))
				count_1++;
		}
		if (count_1 == 0) {
			return mat;
		}
		else
		{
			int normlevel = (1.0 - 0.5468) * DstImg.rows;//单位线y
			int upedge = 0;
			bool flag = true;
			if ((tempy[0] > normlevel) || (Lines.size() == 0))//无有、无无
				flag = false;
			else
			{
				for (int i = 0; i < Lines.size(); i++) {
					if ((tempy[i] < normlevel) && (i == (Lines.size() - 1))) {//有无
						upedge = tempy[i];
					}
					if ((tempy[i] < normlevel) && (tempy[i + 1] > normlevel)) {//有有
						upedge = tempy[i];
						break;
					}
				}
			}
			if (!flag) {
				fy1 = DstImg.rows * 0.408;
			}
			else
			{
				if ((upedge < (normlevel - 15)) && (upedge >(normlevel - 50))) {
					fy1 = upedge;
				}
				else if ((upedge < (normlevel - 50)) && (upedge >(normlevel - 90))) {
					fy1 = upedge + 40;
				}
				else if ((upedge < (normlevel - 90)) && (upedge >(normlevel - 130))) {
					fy1 = upedge + 80;
				}
				else if ((upedge < (normlevel - 130)) && (upedge >(normlevel - 170))) {
					fy1 = upedge + 120;
				}
				else
				{
					fy1 = DstImg.rows * 0.408;
				}
			}
			fy2 = fy1 + 92;
			Mat sDstImg = DstImg.clone();
			roi2 = img(Range(fy1 - 40 - up, fy1 + down + 1), Range(0, DstImg.cols));
			roi3 = img(Range(fy1 - 80 - up, fy1 - 40 + down + 1), Range(0, DstImg.cols));
			roi4 = img(Range(fy1 - 120 - up, fy1 - 80 + down + 1), Range(0, DstImg.cols));
			roi1 = img(Range(fy1 - up, fy2 + 1 + down), Range(0, DstImg.cols));
			mat.push_back(roi1);
			mat.push_back(roi2);
			mat.push_back(roi3);
			mat.push_back(roi4);
		}
	}
	return mat;
}
//切整张支票的目标区域（还未竖直切割,1/0,支票有字,中有白条）
int main_sdc(int argc, char** argv) {
	fstream outFile;
	int up, down, whole = 0;
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
	setPathOfexe(savedir);
	string dir_path(savedir);
	//dir_path += "待分割图片\\";
	//Directory dir;
	string dirpath = "C:\\Users\\Administrator\\Desktop\\数字识别项目\\甲方图片\\样例\\样例";
	vector<string> dirNames;
	getAllFiles(dirpath, dirNames);
	//vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);
	int countpic = 0;
	for (int i1 = 0; i1 < dirNames.size(); i1++)
	{
		vector<string> fileNames;
		Directory dir;
		fileNames = dir.GetListFiles(dirNames[i1], "*.jpg", false);
		for (int i2 = 0; i2 < fileNames.size(); i2++) {
			char Savedir[200];
			strcpy(Savedir, savedir);
			string fileName = fileNames[i2];
			string fileFullName = dirNames[i1] + "\\" + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl << endl;

			Mat img, img0, roi2;
			IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
			int min, max;
			min = 256;
			max = 0;
			int thres = 0;
			img = imread(fileFullName);
			cvtColor(img, img0, CV_BGR2GRAY);
			int numroi = getroicheck2(img0, up, down, img).size();
			if (numroi != 0) {
				roi2 = getroicheck2(img0, up, down, img)[0];
				Mat t = getroicheck2(img0, up, down, img)[1];
				Mat d = getroicheck2(img0, up, down, img)[2];
				Mat q = getroicheck2(img0, up, down, img)[3];
				Mat di, zh;
				if (numroi > 4) {
					di = getroicheck2(img0, up, down, img)[4];
					zh = getroicheck2(img0, up, down, img)[5];
				}
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
				char *chdirname = (char*)dirNames[i1].data();
				char *localdir = strrchr(chdirname, '\\');
				string strlocaldir(localdir);
				string sdir = strlocaldir.substr(1, strlocaldir.length());
				string filename = sdir + "_" + fileName.substr(0, fileName.length() - 4);
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
					memset(save_file1, 0, sizeof(save_file1));
					sprintf(save_file1, "%s%s_%s.jpg", Savedir1, (char*)filename.data(), "3");
					imwrite(save_file1, q);
					if (numroi > 4) {
						memset(save_file1, 0, sizeof(save_file1));
						sprintf(save_file1, "%s%s_%s.jpg", Savedir1, (char*)filename.data(), "4");
						imwrite(save_file1, di);
						memset(save_file1, 0, sizeof(save_file1));
						sprintf(save_file1, "%s%s_%s.jpg", Savedir1, (char*)filename.data(), "5");
						imwrite(save_file1, zh);
					}
				}
			}
			countpic++;
			cout << countpic << endl;
		}
	}

	//waitKey(0);
	return 0;
}