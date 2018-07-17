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
int summ3[10000];


//整合中的横条切割汉字
vector<int> getcutpos1(IplImage *img, int shrink, int shrink1) {
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	vector<int> cutpos;
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	int t = 0;
	for (int i = 0 + shrink; i < width; i++) {
		int sum = 0;
		for (int j = 0 + shrink1; j < height - shrink1; j++) {
			sum = sum + (int)ptr[j*img->widthStep + i];
			if (sum > 0) {
				x1 = i;
				t = 1;
			}
			if (t == 1)
				break;
		}
		if (t == 1)
			break;
	}
	t = 0;
	for (int i = width - shrink - 1; i > 0; i--) {
		int sum = 0;
		for (int j = 0 + shrink1; j < height - shrink1; j++) {
			sum = sum + (int)ptr[j*img->widthStep + i];
			if (sum > 0) {
				x2 = i;
				t = 1;
			}
			if (t == 1)
				break;
		}
		if (t == 1)
			break;
	}
	t = 0;
	for (int i = 0 + shrink1; i < height; i++) {
		int sum = 0;
		for (int j = 0 + shrink; j < width - shrink; j++) {
			sum = sum + (int)ptr[i*img->widthStep + j];
			if (sum > 0) {
				y1 = i;
				t = 1;
			}
			if (t == 1)
				break;
		}
		if (t == 1)
			break;
	}
	t = 0;
	for (int i = height - 1 - shrink1; i > 0; i--) {
		int sum = 0;
		for (int j = 0 + shrink; j < width - shrink; j++) {
			sum = sum + (int)ptr[i*img->widthStep + j];
			if (sum > 0) {
				y2 = i;
				t = 1;
			}
			if (t == 1)
				break;
		}
		if (t == 1)
			break;
	}
	cutpos.push_back(x1);
	cutpos.push_back(x2);
	cutpos.push_back(y1);
	cutpos.push_back(y2);
	return cutpos;
}

void cut1() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 40, 1, 1);
			//imshow("n", img1);
			//waitKey(0);
			IplImage *img2 = &IplImage(img1);
			int x1 = getcutpos1(img2, 0, 0)[0];		//后两个参数是两边和上下预平切，带边缘时第二个参数是20
			int x2 = getcutpos1(img2, 0, 0)[1];
			Mat img3(img2);
			Mat img4 = img3(Range(0, img3.rows), Range(x1, x2 + 1));
			/*imshow("nn", img4);
			waitKey(0);*/
			IplImage *img5 = &IplImage(img4);
			vector<int> fg;
			int pre = 0;
			int next = 0;
			int width1 = img5->width;
			int height1 = img5->height;
			uchar *ptr1 = (uchar *)img5->imageData;
			for (int i = 0; i < width1; i++) {
				for (int j = 0; j < height1; j++) {
					summ3[i] += (int)ptr1[j*img5->widthStep + i];
				}
				if (i == 0)
					pre = summ3[i];
				else
				{
					pre = summ3[i - 1];
					next = summ3[i];
				}
				if ((pre > 0) && (next == 0)) {
					fg.push_back(i);
				}
				if ((pre == 0) && (next > 0)) {
					//if (fg.size() != 0)
					//{
					fg.push_back(i - 1);
					//int fgpoint = ((fg[fg.size() - 1] + fg[fg.size() - 2]) / 2);
					//int cha = fg[fg.size() - 1] - fg[fg.size() - 2];
					//fg.pop_back();
					//fg.pop_back();
					//if (cha > 20)
					//fg.push_back(fgpoint);
					//}
				}
				if (fg.size() > 2) {
					if ((((fg.size() - 1) % 2) == 0)) {
						float w = fg[fg.size() - 2] - fg[fg.size() - 3];
						Mat tempimg = img4(Range(0, img4.rows), Range(fg[fg.size() - 3], fg[fg.size() - 2]));
						IplImage *t = &IplImage(tempimg);
						int y_1 = getcutpos1(t, 0, 0)[2];
						int y_2 = getcutpos1(t, 0, 0)[3];
						float h = y_2 - y_1;
						float rotio = w / h;
						if (rotio < 0.45) {
							if (rotio < 0.3) {			//三瓣的右边处理
								int right = fg[fg.size() - 1];
								int mid = fg[fg.size() - 2];
								fg.pop_back();
								fg.pop_back();
								fg.pop_back();
								fg.pop_back();
								fg.push_back(mid);
								fg.push_back(right);
							}
							else
							{
								fg.pop_back();		//普通左偏旁处理
								fg.pop_back();
							}
						}
						/*if (rotio > 1.5) {
						int fgpoint = ((fg[fg.size() - 2] + fg[fg.size() - 3]) / 2);
						int temp1 = fg[fg.size() - 1];
						int temp2 = fg[fg.size() - 2];
						fg.pop_back();
						fg.pop_back();
						fg.push_back(fgpoint);
						fg.push_back(temp2);
						fg.push_back(temp1);
						}*/
					}
				}
			}
			//fg.pop_back();
			//fg.push_back(width1);
			//fg.insert(fg.begin(), 0);
			Mat *matroi = new Mat[(fg.size() / 2)];
			//string output = "C:\\Users\\Administrator\\Desktop\\";
			//rmdir(output.c_str());
			char save_file[200];
			//char *Savdir = (char*)output.data();
			//mkdir(Savdir);
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			for (int j = 0; j < (fg.size() / 2); j++) {
				matroi[j] = img4(Range(0, img4.rows), Range(fg[j * 2], fg[j * 2 + 1]));
				IplImage *temp = &IplImage(matroi[j]);
				int x1 = getcutpos1(temp, 0, 0)[0];
				int x2 = getcutpos1(temp, 0, 0)[1];
				int y1 = getcutpos1(temp, 0, 0)[2];
				int y2 = getcutpos1(temp, 0, 0)[3];
				Mat fina = matroi[j](Range(y1, y2 + 1), Range(x1, x2 + 1));
				//sprintf(save_file, "%s%d.png", Savedirclone, j);
				sprintf(save_file, "%s%d.jpg", Savedir, j + 1);
				//Mat res = matroi[j];
				bitwise_not(fina, fina);
				if (fina.cols > 1)
					imwrite(save_file, fina);
			}
			memset(summ3, 0, 10000);;
		}
	}
}

void cut2() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
	Directory dir;
	vector<string> fileNames;
	getAllFiles(dir_path, fileNames);
	ofstream in;
	in.open("C:\\Users\\Administrator\\Desktop\\nbOCR_2018.1.26\\nbOCR_2018.1.25\\nbOCR\\Debug\\300cutSize.txt", ios::trunc);//切割结果数量写入文件

	map<int, int> cutResultSize;
	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		strcpy(Savedir, savedir);
		string fileName = fileNames[ii];
		string ff = fileName.substr(fileName.length() - 4, fileName.length());
		if (ff.compare(".png") == 0) {
			cout << "file name:" << fileName << endl;

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 8, 1, 1);
			Mat imgclone = img1.clone();
			vector<vector<Point>> contours;
			findContours(img1,
				contours,   //轮廓的数组  
				CV_RETR_EXTERNAL,
				//CV_RETR_TREE,   //获取内外轮廓  
				CV_CHAIN_APPROX_NONE);
			//移除过长或过短的轮廓  
			//removeShortContours(contours, 0, 500);
			Mat contoursImg = Mat(img1.rows, img1.cols, CV_8U);
			drawContours(contoursImg, contours);
			//imshow("contoursImg", contoursImg);
			//将联通区转换为坐标表示并按x1升序排序
			vector<int*> coordinates = CCPoints2Coordinates(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			/*Mat ccaImg = cutred.clone();
			for (int i = 0; i < coordinates.size(); i++) {
			rectangle(ccaImg, Point(coordinates.at(i)[0], coordinates.at(i)[2]), Point(coordinates.at(i)[1], coordinates.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			imshow("cca", ccaImg);*/
			vector<int*> combined = combineOverlapComponents(coordinates);
			Mat splig = imgclone.clone();
			/*Mat splig1 = imgclone.clone();
			for (int i = 0; i < combined.size(); i++) {
			rectangle(splig1, Point(combined.at(i)[0], combined.at(i)[2]), Point(combined.at(i)[1], combined.at(i)[3]), Scalar(255, 255, 255), 0.2);
			}
			imshow("1", splig1);*/
			vector<int*> newcombined = combinecombined(combined, splig);
			Mat splitImg = imgclone.clone();
			/*for (int i = 0; i < newcombined.size(); i++) {
			rectangle(splitImg, Point(newcombined.at(i)[0], newcombined.at(i)[2]), Point(newcombined.at(i)[1], newcombined.at(i)[3]), Scalar(255, 255, 255), 0.2);
			}*/
			//cout << "0:"<< (float)(newcombined[3][1]- newcombined[3][0])/(float)(newcombined[3][3] - newcombined[3][2]) << endl;
			//cout << "0:" << (float)(newcombined[4][1] - newcombined[4][0]) / (float)(newcombined[4][3] - newcombined[4][2]) << endl;
			//imshow("split", splitImg);
			//waitKey(0);

			//将切割数量写入文件
			int cutSize = newcombined.size();
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			int num;
			const char *a;
			a = nn.c_str();
			sscanf(a, "%d", &num);
			cutResultSize.insert(make_pair(num, cutSize));

			Mat fgImg = imgclone.clone();
			vector<int> cutpoint;
			saverec(newcombined, fileName, Savedir, fgImg);
		}
	}
	for (map<int, int>::iterator i = cutResultSize.begin(); i != cutResultSize.end(); i++)
	{
		in << (*i).second << "\n";
	}
	in.close();//关闭文件

}

void cut3() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 8, 1, 1);
			//imshow("n", img1);
			//waitKey(0);
			IplImage *img2 = &IplImage(img1);
			int x1 = getcutpos(img2, 0)[0];		//后两个参数是两边和上下预平切，带边缘时第二个参数是20
			int x2 = getcutpos(img2, 0)[1];
			Mat img3(img2);
			Mat img4 = img3(Range(0, img3.rows), Range(x1, x2 + 1));
			/*imshow("nn", img4);
			waitKey(0);*/
			IplImage *img5 = &IplImage(img4);
			vector<int> fg;
			int pre = 0;
			int next = 0;
			int width1 = img5->width;
			int height1 = img5->height;
			uchar *ptr1 = (uchar *)img5->imageData;
			for (int i = 0; i < width1; i++) {
				for (int j = 0; j < height1; j++) {
					summ3[i] += (int)ptr1[j*img5->widthStep + i];
				}
				if (i == 0)
					pre = summ3[i];
				else
				{
					pre = summ3[i - 1];
					next = summ3[i];
				}
				if ((pre > 0) && (next == 0)) {
					fg.push_back(i);
				}
				if ((pre == 0) && (next > 0)) {
					fg.push_back(i - 1);
				}
			}
			fg.push_back(width1);
			Mat *matroi = new Mat[(fg.size() / 2)];
			char save_file[200];
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			int numpic = 0;
			vector<int> wid;
			vector<Mat> fresmat;
			for (int j = 0; j < (fg.size() / 2); j++) {
				matroi[j] = img4(Range(0, img4.rows), Range(fg[j * 2], fg[j * 2 + 1]));
				IplImage *temp = &IplImage(matroi[j]);
				int x1 = getcutpos(temp, 0)[0];
				int x2 = getcutpos(temp, 0)[1];
				int y1 = getcutpos(temp, 0)[2];
				int y2 = getcutpos(temp, 0)[3];
				Mat fina = matroi[j](Range(y1, y2), Range(x1, x2));
				if (fina.cols != 0) {
					fresmat.push_back(fina);
					wid.push_back(x2 - x1);
				}
			}
			int midwid = Middle(wid, wid.size());
			for (int fsp = 0; fsp < fresmat.size(); fsp++) {
				Mat matfsp = fresmat[fsp];
				int cwid = matfsp.cols;
				float cnum = cwid * 1.0 / midwid * 1.0;
				if (cnum < 1.5) {
					IplImage *a1ipl = &IplImage(matfsp);
					int x1 = getcutpos(a1ipl, 0)[0];
					int x2 = getcutpos(a1ipl, 0)[1];
					int y1 = getcutpos(a1ipl, 0)[2];
					int y2 = getcutpos(a1ipl, 0)[3];
					Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
					bitwise_not(a1res, a1res);
					sprintf(save_file, "%s%d.jpg", Savedir, numpic);
					imwrite(save_file, a1res);
					numpic++;
				}
				else
				{
					int maxsumdis = -9999;		//连线最长
					int suitr = 0;
					Mat finaclone = matfsp.clone();
					for (int r = 0; r < 20; r++) {
						Mat rotamat = getRotationMatrix2D(Point2f(finaclone.rows, 0), r, 1);
						Mat dstresult;
						Scalar borderColor = Scalar(0);
						warpAffine(finaclone, dstresult, rotamat, finaclone.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
						Mat d1 = dstresult.clone();
						vector<int*> split = morecut(d1);
						int sumdis = 0;
						for (int sp = 0; sp < split.size() - 1; sp++) {
							sumdis = sumdis + (split[sp + 1][1] - split[sp][0] -
								((split[sp][1] - split[sp][0]) / 2) - ((split[sp + 1][1] - split[sp + 1][0]) / 2));
						}
						if (sumdis > maxsumdis) {
							maxsumdis = sumdis;
							suitr = r;
						}
					}
					Mat rotamat = getRotationMatrix2D(Point2f(matfsp.rows, 0), suitr, 1);
					Mat dstresult;
					Scalar borderColor = Scalar(0);
					warpAffine(matfsp, dstresult, rotamat, matfsp.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
					Mat d2 = dstresult.clone();
					vector<int*> split = morecut(d2);
					for (int i = 0; i < split.size(); i++) {
						//int cutpos = split[i][1];
						//bool flag = SingleCut(dstresult, cutpos);
						//if (flag) {
							Mat temp = dstresult(Range(split[i][2], split[i][3]), Range(split[i][0], split[i][1]));
							IplImage *a1ipl = &IplImage(temp);
							int x1 = getcutpos(a1ipl, 0)[0];
							int x2 = getcutpos(a1ipl, 0)[1];
							int y1 = getcutpos(a1ipl, 0)[2];
							int y2 = getcutpos(a1ipl, 0)[3];
							Mat a1res = temp(Range(y1, y2), Range(x1, x2));
							bitwise_not(a1res, a1res);
							sprintf(save_file, "%s%d.jpg", Savedir, numpic);
							imwrite(save_file, a1res);
							numpic++;
						//}
						/*else
						{
							IplImage *a1ipl = &IplImage(matfsp);
							int x1 = getcutpos(a1ipl, 0)[0];
							int x2 = getcutpos(a1ipl, 0)[1];
							int y1 = getcutpos(a1ipl, 0)[2];
							int y2 = getcutpos(a1ipl, 0)[3];
							Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
							bitwise_not(a1res, a1res);
							sprintf(save_file, "%s%d.jpg", Savedir, numpic);
							imwrite(save_file, a1res);
							numpic++;
						}*/
					}
				}

			}
		}
		memset(summ3, 0, 10000);
	}
}

void cut4() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 8, 1, 1);
			//imshow("n", img1);
			//waitKey(0);
			IplImage *img2 = &IplImage(img1);
			int x1 = getcutpos(img2, 0)[0];		//后两个参数是两边和上下预平切，带边缘时第二个参数是20
			int x2 = getcutpos(img2, 0)[1];
			Mat img3(img2);
			Mat img4 = img3(Range(0, img3.rows), Range(x1, x2 + 1));
			/*imshow("nn", img4);
			waitKey(0);*/
			IplImage *img5 = &IplImage(img4);
			vector<int> fg = maptolookcut(img5);
			
			Mat *matroi = new Mat[(fg.size() / 2)];
			char save_file[200];
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			int numpic = 0;
			vector<int> wid;
			vector<Mat> fresmat;
			for (int j = 0; j < (fg.size() / 2); j++) {
				matroi[j] = img4(Range(0, img4.rows), Range(fg[j * 2], fg[j * 2 + 1]));
				IplImage *temp = &IplImage(matroi[j]);
				int x1 = getcutpos(temp, 0)[0];
				int x2 = getcutpos(temp, 0)[1];
				int y1 = getcutpos(temp, 0)[2];
				int y2 = getcutpos(temp, 0)[3];
				Mat fina = matroi[j](Range(y1, y2 + 1), Range(x1, x2 + 1));
				//imshow("vva", fina);
				//waitKey();
				if (fina.cols != 0) {
					int maxsumdis = -9999;		
					int suitr = 0;
					for (int r = 0; r < 20; r++) {
						Mat rotamat = getRotationMatrix2D(Point2f(fina.rows, 0), r, 1);
						Mat dstresult;
						Scalar borderColor = Scalar(0);
						warpAffine(fina, dstresult, rotamat, fina.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
						Mat dstclone = dstresult.clone();
						IplImage *d1 = &IplImage(dstclone);
						vector<int> split = maptolookcut(d1);
						int spsize = split.size();
						if (spsize > maxsumdis) {
							maxsumdis = spsize;
							suitr = r;
						}
					}
				

					
						Mat rotamat = getRotationMatrix2D(Point2f(fina.rows, 0), suitr, 1);
						Mat dstresult;
						Scalar borderColor = Scalar(0);
						warpAffine(fina, dstresult, rotamat, fina.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
						//imshow("vva", dstresult);
						//waitKey();
						Mat dstresultclone = dstresult.clone();
						IplImage* d2 = &IplImage(dstresultclone);
						vector<int> split = maptolookcut(d2);
						for (int j = 0; j < (split.size() / 2); j++) {
							Mat t1 = dstresult(Range(0, dstresult.rows), Range(split[j * 2], split[j * 2 + 1]));
							IplImage *temp = &IplImage(t1);
							int x1 = getcutpos(temp, 0)[0];
							int x2 = getcutpos(temp, 0)[1];
							int y1 = getcutpos(temp, 0)[2];
							int y2 = getcutpos(temp, 0)[3];
							Mat fina = t1(Range(y1, y2 + 1), Range(x1, x2 + 1));
							sprintf(save_file, "%s%d.jpg", Savedir, numpic);
							bitwise_not(fina, fina);
							if (fina.cols != 0) {
								imwrite(save_file, fina);
								numpic++;
							}
						}

					}
				}
				
			//}
		}
	}
}

void cut5() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 10, 1, 1);
			vector<vector<Point>> contours1;
			Mat img1clone = img1.clone();
			findContours(img1,
				contours1,
				CV_RETR_EXTERNAL,
				//CV_RETR_TREE,
				CV_CHAIN_APPROX_NONE);
			vector<vector<Point>> contours = removeShortContoursfornumber(contours1, img1.rows);
			vector<int*> coordinates = CCPoints2Coordinates1(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			vector<int*> coordinates1 = combinecoord(coordinates);
			vector<int> wid;
			vector<int> contourwid;
			//vector<int> area;
			vector<Mat> tempMat;
			for (int i = 0; i < coordinates1.size(); i++) {
				int x1 = coordinates1[i][0];
				int x2 = coordinates1[i][1];
				int y1 = coordinates1[i][2];
				int y2 = coordinates1[i][3];
				Mat temp = img1clone(Range(y1, y2 + 1), Range(x1, x2 + 1));

				Mat tempclone = temp.clone();
				Mat tempclone1 = temp.clone();
				vector<Point> finalcontour;
				for (int i1 = 0; i1 < contours[coordinates1[i][4]].size(); i1++) {
					finalcontour.push_back(contours[coordinates1[i][4]][i1]);
				}
				if (coordinates1[i][5] != -1) {
					for (int i2 = 0; i2 < contours[coordinates1[i][5]].size(); i2++) {
						finalcontour.push_back(contours[coordinates1[i][5]][i2]);
					}
				}
				sort(finalcontour.begin(), finalcontour.end(), yCmp);
				int yup = finalcontour[0].y;
				int ydown = finalcontour[finalcontour.size() - 1].y;
				vector<vector<int>> filled = contoursRows(finalcontour, yup, ydown);
				for (int j = 0; j < tempclone1.rows; j++) {
					for (int l = 0; l < tempclone1.cols; l++) {
						if (filled[j].size() == 1) {
							int x = filled[j][0];
							if (l != (x - x1)) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
						else if (filled[j].size() == 0)
						{
							tempclone1.at<uchar>(j, l) = 0;
						}
						else
						{
							int xl = filled[j][0];
							int xr = filled[j][1];
							if ((l < (xl - x1)) || (l >(xr - x1))) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
					}
				}
				wid.push_back(tempclone1.cols);
				tempMat.push_back(tempclone1);
				contourwid.push_back(finalcontour.size());
				//vector<vector<Point>> contours2;
				//Mat recontour = tempclone1.clone();
				//findContours(recontour,
				//	contours2,
				//	//CV_RETR_EXTERNAL,
				//	CV_RETR_TREE,
				//	CV_CHAIN_APPROX_NONE);
				//vector<Point> recons;
				//for (int i = 0; i < contours2.size(); i++)
				//	for (int j = 0; j < contours2[i].size(); j++)
				//		recons.push_back(contours2[i][j]);
				//area.push_back(contourArea(recons));
			}
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			int midwid = Middle(wid, wid.size());
			int midconwid = Middle(contourwid, contourwid.size());
			//int midarea = Middle(area, area.size());
			//sort(contourwid.begin(), contourwid.end(), widthCmp);
			//sort(wid.begin(), wid.end(), widthCmp);
			//sort(area.begin(), area.end(), widthCmp);
			int numpic = 0;
			for (int fsp = 0; fsp < tempMat.size(); fsp++) {
				char save_file[200];
				Mat matfsp = tempMat[fsp];
				int cwid = matfsp.cols;
				int conwid = contourwid[fsp];
				float cnum = conwid * 1.0 / midconwid * 1.0;
				float colnum = cwid * 1.0 / midwid * 1.0;
				if (colnum < 1.5) {
					if (cnum < 1.5) {
						IplImage *a1ipl = &IplImage(matfsp);
						int x1 = getcutpos(a1ipl, 0)[0];
						int x2 = getcutpos(a1ipl, 0)[1];
						int y1 = getcutpos(a1ipl, 0)[2];
						int y2 = getcutpos(a1ipl, 0)[3];
						Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
						bitwise_not(a1res, a1res);
						sprintf(save_file, "%s%d.png", Savedir, numpic);
						imwrite(save_file, a1res);
						numpic++;
					}
					else
					{
						int maxsumdis = -9999;		//连线最长
						int suitr = 0;
						Mat finaclone = matfsp.clone();
						for (int r = 0; r < 20; r++) {
							Mat rotamat = getRotationMatrix2D(Point2f(finaclone.rows, 0), r, 1);
							Mat dstresult;
							Scalar borderColor = Scalar(0);
							warpAffine(finaclone, dstresult, rotamat, finaclone.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
							Mat d1 = dstresult.clone();
							vector<int*> split = morecut(d1);
							int sumdis = 0;
							for (int sp = 0; sp < split.size() - 1; sp++) {
								sumdis = sumdis + (split[sp + 1][1] - split[sp][0] -
									((split[sp][1] - split[sp][0]) / 2) - ((split[sp + 1][1] - split[sp + 1][0]) / 2));
							}
							if (sumdis > maxsumdis) {
								maxsumdis = sumdis;
								suitr = r;
							}
						}
						Mat rotamat = getRotationMatrix2D(Point2f(matfsp.rows, 0), suitr, 1);
						Mat dstresult;
						Scalar borderColor = Scalar(0);
						warpAffine(matfsp, dstresult, rotamat, matfsp.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
						Mat d2 = dstresult.clone();
						vector<int*> split = morecut(d2);
						for (int i = 0; i < split.size(); i++) {
							Mat temp = dstresult(Range(split[i][2], split[i][3]), Range(split[i][0], split[i][1]));
							IplImage *a1ipl = &IplImage(temp);
							int x1 = getcutpos(a1ipl, 0)[0];
							int x2 = getcutpos(a1ipl, 0)[1];
							int y1 = getcutpos(a1ipl, 0)[2];
							int y2 = getcutpos(a1ipl, 0)[3];
							Mat a1res = temp(Range(y1, y2), Range(x1, x2));
							bitwise_not(a1res, a1res);
							sprintf(save_file, "%s%d.png", Savedir, numpic);
							imwrite(save_file, a1res);
							numpic++;
						}
					}
				}
				else
				{
					if ((cnum < 1.5) && (colnum < 1.7)) {
						IplImage *a1ipl = &IplImage(matfsp);
						int x1 = getcutpos(a1ipl, 0)[0];
						int x2 = getcutpos(a1ipl, 0)[1];
						int y1 = getcutpos(a1ipl, 0)[2];
						int y2 = getcutpos(a1ipl, 0)[3];
						Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
						bitwise_not(a1res, a1res);
						sprintf(save_file, "%s%d.png", Savedir, numpic);
						imwrite(save_file, a1res);
						numpic++;
					}
					else
					{
						int maxsumdis = -9999;		//连线最长
						int suitr = 0;
						Mat finaclone = matfsp.clone();
						for (int r = 0; r < 20; r++) {
							Mat rotamat = getRotationMatrix2D(Point2f(finaclone.rows, 0), r, 1);
							Mat dstresult;
							Scalar borderColor = Scalar(0);
							warpAffine(finaclone, dstresult, rotamat, finaclone.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
							Mat d1 = dstresult.clone();
							vector<int*> split = morecut(d1);
							int sumdis = 0;
							for (int sp = 0; sp < split.size() - 1; sp++) {
								sumdis = sumdis + (split[sp + 1][1] - split[sp][0] -
									((split[sp][1] - split[sp][0]) / 2) - ((split[sp + 1][1] - split[sp + 1][0]) / 2));
							}
							if (sumdis > maxsumdis) {
								maxsumdis = sumdis;
								suitr = r;
							}
						}
						Mat rotamat = getRotationMatrix2D(Point2f(matfsp.rows, 0), suitr, 1);
						Mat dstresult;
						Scalar borderColor = Scalar(0);
						warpAffine(matfsp, dstresult, rotamat, matfsp.size() * 2, INTER_LINEAR, BORDER_CONSTANT, borderColor);
						Mat d2 = dstresult.clone();
						vector<int*> split = morecut(d2);
						for (int i = 0; i < split.size(); i++) {
							Mat temp = dstresult(Range(split[i][2], split[i][3]), Range(split[i][0], split[i][1]));
							IplImage *a1ipl = &IplImage(temp);
							int x1 = getcutpos(a1ipl, 0)[0];
							int x2 = getcutpos(a1ipl, 0)[1];
							int y1 = getcutpos(a1ipl, 0)[2];
							int y2 = getcutpos(a1ipl, 0)[3];
							Mat a1res = temp(Range(y1, y2), Range(x1, x2));
							bitwise_not(a1res, a1res);
							sprintf(save_file, "%s%d.png", Savedir, numpic);
							imwrite(save_file, a1res);
							numpic++;
						}
					}
					
				}
			}
		}
	}
}

void cut6() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			//deleteLines(img1);		//实际支票去线过程
			RemoveSmallRegion(img1, img1, 50, 1, 1);
			vector<vector<Point>> contours1;
			Mat img1clone = img1.clone();
			findContours(img1,
				contours1,
				CV_RETR_EXTERNAL,
				//CV_RETR_TREE,
				CV_CHAIN_APPROX_NONE);
			vector<vector<Point>> contours = removeShortContoursfornumber(contours1, img1.rows);
			vector<int*> coordinates = CCPoints2Coordinates1(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			vector<int*> coordinates1 = combinecoord(coordinates);
			vector<int> wid;
			vector<Mat> tempMat;
			for (int i = 0; i < coordinates1.size(); i++) {
				int x1 = coordinates1[i][0];
				int x2 = coordinates1[i][1];
				int y1 = coordinates1[i][2];
				int y2 = coordinates1[i][3];
				Mat temp = img1clone(Range(y1, y2 + 1), Range(x1, x2 + 1));

				Mat tempclone = temp.clone();
				Mat tempclone1 = temp.clone();
				//vector<vector<Point>> contours1;
				//findContours(tempclone,
				//	contours1,
				//	CV_RETR_EXTERNAL,
				//	//CV_RETR_TREE,
				//	CV_CHAIN_APPROX_NONE);
				//vector<Point> finalcontour = getMaxContours(contours1);
				vector<Point> finalcontour;
				for (int i1 = 0; i1 < contours[coordinates1[i][4]].size(); i1++) {
					finalcontour.push_back(contours[coordinates1[i][4]][i1]);
				}
				if (coordinates1[i][5] != -1) {
					for (int i2 = 0; i2 < contours[coordinates1[i][5]].size(); i2++) {
						finalcontour.push_back(contours[coordinates1[i][5]][i2]);
					}
				}
				sort(finalcontour.begin(), finalcontour.end(), yCmp);
				int yup = finalcontour[0].y;
				int ydown = finalcontour[finalcontour.size() - 1].y;
				vector<vector<int>> filled = contoursRows(finalcontour, yup, ydown);
				for (int j = 0; j < tempclone1.rows; j++) {
					for (int l = 0; l < tempclone1.cols; l++) {
						if (filled[j].size() == 1) {
							int x = filled[j][0];
							if (l != (x - x1)) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
						else if (filled[j].size() == 0)
						{
							tempclone1.at<uchar>(j, l) = 0;
						}
						else
						{
							int xl = filled[j][0];
							int xr = filled[j][1];
							if ((l < (xl - x1)) || (l >(xr - x1))) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
					}
				}
				wid.push_back(tempclone1.cols);
				tempMat.push_back(tempclone1);
				//imshow("vsvs", tempclone1);
				//waitKey();
			}
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			int numpic = 0;
			for (int fsp = 0; fsp < tempMat.size(); fsp++) {
				char save_file[200];
				Mat matfsp = tempMat[fsp];
				IplImage *a1ipl = &IplImage(matfsp);
				int x1 = getcutpos(a1ipl, 0)[0];
				int x2 = getcutpos(a1ipl, 0)[1];
				int y1 = getcutpos(a1ipl, 0)[2];
				int y2 = getcutpos(a1ipl, 0)[3];
				Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
				bitwise_not(a1res, a1res);
				sprintf(save_file, "%s%d.jpg", Savedir, numpic);
				imwrite(save_file, a1res);
				numpic++;
			}
		}
	}
}

void cut7() {
	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	dir_path += "待分割图片";
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

			Mat img1, roi2;
			Mat img0 = imread(fileName);
			Mat img_ud = img0(Range(img0.rows * 0.38, img0.rows * 0.821), Range(0, img0.cols));
			cvtColor(img_ud, img1, CV_RGB2GRAY);
			threshold(img1, img1, 0, 255, CV_THRESH_OTSU);
			bitwise_not(img1, img1);
			RemoveSmallRegion(img1, img1, 15, 1, 1);
			
			Mat dilimg;					//膨胀修补断裂
			Mat ele = getStructuringElement(MORPH_RECT, Size(2.5, 2.5));
			dilate(img1, dilimg, ele);
			imshow("nbh", dilimg);
			//waitKey();
			vector<vector<Point>> contours1;
			Mat img1clone = dilimg.clone();
			findContours(dilimg,
				contours1,
				CV_RETR_EXTERNAL,
				//CV_RETR_TREE,
				CV_CHAIN_APPROX_NONE);
			vector<vector<Point>> contours = removeShortContoursfornumber(contours1, img1.rows);
			vector<int*> coordinates = CCPoints2Coordinates1(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			vector<int*> coordinates1 = combinecoord(coordinates);
			vector<int> wid;
			vector<Mat> tempMat;
			for (int i = 0; i < coordinates1.size(); i++) {
				int x1 = coordinates1[i][0];
				int x2 = coordinates1[i][1];
				int y1 = coordinates1[i][2];
				int y2 = coordinates1[i][3];
				Mat temp = img1clone(Range(y1, y2 + 1), Range(x1, x2 + 1));

				Mat tempclone = temp.clone();
				Mat tempclone1 = temp.clone();
				vector<Point> finalcontour;
				for (int i1 = 0; i1 < contours[coordinates1[i][4]].size(); i1++) {
					finalcontour.push_back(contours[coordinates1[i][4]][i1]);
				}
				if (coordinates1[i][5] != -1) {
					for (int i2 = 0; i2 < contours[coordinates1[i][5]].size(); i2++) {
						finalcontour.push_back(contours[coordinates1[i][5]][i2]);
					}
				}
				sort(finalcontour.begin(), finalcontour.end(), yCmp);
				int yup = finalcontour[0].y;
				int ydown = finalcontour[finalcontour.size() - 1].y;
				vector<vector<int>> filled = contoursRows(finalcontour, yup, ydown);
				for (int j = 0; j < tempclone1.rows; j++) {
					for (int l = 0; l < tempclone1.cols; l++) {
						if (filled[j].size() == 1) {
							int x = filled[j][0];
							if (l != (x - x1)) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
						else if (filled[j].size() == 0)
						{
							tempclone1.at<uchar>(j, l) = 0;
						}
						else
						{
							int xl = filled[j][0];
							int xr = filled[j][1];
							if ((l < (xl - x1)) || (l >(xr - x1))) {
								tempclone1.at<uchar>(j, l) = 0;
							}
						}
					}
				}
				wid.push_back(tempclone1.cols);
				tempMat.push_back(tempclone1);
				//imshow("vsvs", tempclone1);
				//waitKey();
			}
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 5);
			string dir2 = "分割\\" + nn + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			int numpic = 0;
			for (int fsp = 0; fsp < tempMat.size(); fsp++) {
				char save_file[200];
				Mat matfsp = tempMat[fsp];
				IplImage *a1ipl = &IplImage(matfsp);
				int x1 = getcutpos(a1ipl, 0)[0];
				int x2 = getcutpos(a1ipl, 0)[1];
				int y1 = getcutpos(a1ipl, 0)[2];
				int y2 = getcutpos(a1ipl, 0)[3];
				Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
				bitwise_not(a1res, a1res);
				sprintf(save_file, "%s%d.png", Savedir, numpic);
				if (a1res.rows > 15) {
					imwrite(save_file, a1res);
					numpic++;
				}
			}
		}
	}
}
int main_dzb() {
	//cut1();		//投影切汉字
	//cut2();			//投影加纵横比切汉字*
	//cut3();		//投影+纵横比切黏连数字*
	//cut4();			//投影+旋转投影切数字*
	cut5();		//联通区切图+几种切黏连数字*
	//cut6();		//联通区切不黏连数字
	//cut7();			//切自己打印支票不黏连数字
	return 0;
}



/*Mat matfspclone = matfsp.clone();		//旋转找轮廓最细切
Mat matfsp1;
vector<int> angleandpos = getWrapandCutpos(matfspclone);
copyMakeBorder(matfsp, matfsp1, matfsp.rows / 4, matfsp.rows / 4,
matfsp.cols / 4, matfsp.cols / 4, BORDER_CONSTANT, Scalar(0));
Mat rotamat = getRotationMatrix2D(Point2f(matfsp1.rows / 2, matfsp1.cols / 2), angleandpos[0], 1);
Mat dstresult;
Scalar borderColor = Scalar(0);
warpAffine(matfsp1, dstresult, rotamat, matfsp1.size(), INTER_LINEAR, BORDER_CONSTANT, borderColor);
int cutpos = angleandpos[1];
Mat left = dstresult(Range(0, dstresult.rows), Range(0, cutpos));
Mat right = dstresult(Range(0, dstresult.rows), Range(cutpos, dstresult.cols));
IplImage *leftipl = &IplImage(left);
int xl1 = getcutpos(leftipl, 0)[0];
int xl2 = getcutpos(leftipl, 0)[1];
int yl1 = getcutpos(leftipl, 0)[2];
int yl2 = getcutpos(leftipl, 0)[3];
Mat leftres = left(Range(yl1, yl2), Range(xl1, xl2));
IplImage *rightipl = &IplImage(right);
int xr1 = getcutpos(rightipl, 0)[0];
int xr2 = getcutpos(rightipl, 0)[1];
int yr1 = getcutpos(rightipl, 0)[2];
int yr2 = getcutpos(rightipl, 0)[3];
Mat rightres = right(Range(yr1, yr2), Range(xr1, xr2));
bitwise_not(leftres, leftres);
bitwise_not(rightres, rightres);
sprintf(save_file, "%s%d.jpg", Savedir, numpic);
imwrite(save_file, leftres);
numpic++;
sprintf(save_file, "%s%d.jpg", Savedir, numpic);
imwrite(save_file, rightres);
numpic++;*/


/*Mat matfspclone = matfsp.clone();		//波峰数量确定切不切
int crestnum = Crest(matfsp);
if (crestnum > 1) {
int cutpos = MapsumtoCut(matfsp);
Mat left = matfspclone(Range(0, matfspclone.rows), Range(0, cutpos));
Mat right = matfspclone(Range(0, matfspclone.rows), Range(cutpos, matfspclone.cols));
IplImage *leftipl = &IplImage(left);
int xl1 = getcutpos(leftipl, 0)[0];
int xl2 = getcutpos(leftipl, 0)[1];
int yl1 = getcutpos(leftipl, 0)[2];
int yl2 = getcutpos(leftipl, 0)[3];
Mat leftres = left(Range(yl1, yl2), Range(xl1, xl2));
IplImage *rightipl = &IplImage(right);
int xr1 = getcutpos(rightipl, 0)[0];
int xr2 = getcutpos(rightipl, 0)[1];
int yr1 = getcutpos(rightipl, 0)[2];
int yr2 = getcutpos(rightipl, 0)[3];
Mat rightres = right(Range(yr1, yr2), Range(xr1, xr2));
bitwise_not(leftres, leftres);
bitwise_not(rightres, rightres);
sprintf(save_file, "%s%d.jpg", Savedir, numpic);
imwrite(save_file, leftres);
numpic++;
sprintf(save_file, "%s%d.jpg", Savedir, numpic);
imwrite(save_file, rightres);
numpic++;
}
else
{
IplImage *a1ipl = &IplImage(matfspclone);
int x1 = getcutpos(a1ipl, 0)[0];
int x2 = getcutpos(a1ipl, 0)[1];
int y1 = getcutpos(a1ipl, 0)[2];
int y2 = getcutpos(a1ipl, 0)[3];
Mat a1res = matfspclone(Range(y1, y2), Range(x1, x2));
bitwise_not(a1res, a1res);
sprintf(save_file, "%s%d.jpg", Savedir, numpic);
imwrite(save_file, a1res);
numpic++;
}*/


//Mat matfspclone = matfsp.clone();			//根据交叉点和单笔划区域切
//Mat singleStrokemat = matfsp.clone();
//bitwise_not(matfsp, matfsp);
//cvtColor(matfsp, matfsp, CV_GRAY2BGR);
//Mat thin = getthinImage(matfsp);
//filterOver(thin);
//vector<Point> points = getPoints(thin, 6, 9, 6);
///*thin = thin * 255;
//vector<cv::Point>::iterator it = points.begin();
//for (; it != points.end(); it++)
//{
//	circle(thin, *it, 2, 255, 1);
//}*/
//if (points.size() != 0) {
//	int midx = matfsp.cols / 2;
//	sort(points.begin(), points.end(), xCmp);
//	int xindex = 0;
//	int minpx = 9999;
//	for (int pi = 0; pi < points.size(); pi++) {
//		int cha = abs(points[pi].x - midx);
//		if (cha < minpx) {
//			minpx = cha;
//			xindex = pi;
//		}
//	}
//	int cutpos = points[xindex].x;
//	bool singlecut = SingleCut(singleStrokemat, cutpos);
//	if (singlecut) {
//		Mat left = matfspclone(Range(0, matfspclone.rows), Range(0, cutpos));
//		Mat right = matfspclone(Range(0, matfspclone.rows), Range(cutpos, matfspclone.cols));
//		IplImage *leftipl = &IplImage(left);
//		int xl1 = getcutpos(leftipl, 0)[0];
//		int xl2 = getcutpos(leftipl, 0)[1];
//		int yl1 = getcutpos(leftipl, 0)[2];
//		int yl2 = getcutpos(leftipl, 0)[3];
//		Mat leftres = left(Range(yl1, yl2), Range(xl1, xl2));
//		IplImage *rightipl = &IplImage(right);
//		int xr1 = getcutpos(rightipl, 0)[0];
//		int xr2 = getcutpos(rightipl, 0)[1];
//		int yr1 = getcutpos(rightipl, 0)[2];
//		int yr2 = getcutpos(rightipl, 0)[3];
//		Mat rightres = right(Range(yr1, yr2), Range(xr1, xr2));
//		bitwise_not(leftres, leftres);
//		bitwise_not(rightres, rightres);
//		sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//		imwrite(save_file, leftres);
//		numpic++;
//		sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//		imwrite(save_file, rightres);
//		numpic++;
//		/*imshow("vdbg", thin);
//		waitKey();*/
//	}
//	else
//	{
//		IplImage *a1ipl = &IplImage(matfspclone);
//		int x1 = getcutpos(a1ipl, 0)[0];
//		int x2 = getcutpos(a1ipl, 0)[1];
//		int y1 = getcutpos(a1ipl, 0)[2];
//		int y2 = getcutpos(a1ipl, 0)[3];
//		Mat a1res = matfspclone(Range(y1, y2), Range(x1, x2));
//		bitwise_not(a1res, a1res);
//		sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//		imwrite(save_file, a1res);
//		numpic++;
//	}
//}
//else
//{
//	IplImage *a1ipl = &IplImage(matfspclone);
//	int x1 = getcutpos(a1ipl, 0)[0];
//	int x2 = getcutpos(a1ipl, 0)[1];
//	int y1 = getcutpos(a1ipl, 0)[2];
//	int y2 = getcutpos(a1ipl, 0)[3];
//	Mat a1res = matfspclone(Range(y1, y2), Range(x1, x2));
//	bitwise_not(a1res, a1res);
//	sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//	imwrite(save_file, a1res);
//	numpic++;
//}

//IplImage *a1ipl = &IplImage(matfsp);		//直接存
//int x1 = getcutpos(a1ipl, 0)[0];
//int x2 = getcutpos(a1ipl, 0)[1];
//int y1 = getcutpos(a1ipl, 0)[2];
//int y2 = getcutpos(a1ipl, 0)[3];
//Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
//bitwise_not(a1res, a1res);
//sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//imwrite(save_file, a1res);
//numpic++;

//Mat matfspclone = matfsp.clone();		//投射最少区域
//int cutpos = MapsumtoCut(matfsp);
//bool singlecut = SingleCut(matfsp, cutpos);
//if ((cutpos != 0)) {
//	Mat left = matfspclone(Range(0, matfspclone.rows), Range(0, cutpos));
//	Mat right = matfspclone(Range(0, matfspclone.rows), Range(cutpos, matfspclone.cols));
//	IplImage *leftipl = &IplImage(left);
//	int xl1 = getcutpos(leftipl, 0)[0];
//	int xl2 = getcutpos(leftipl, 0)[1];
//	int yl1 = getcutpos(leftipl, 0)[2];
//	int yl2 = getcutpos(leftipl, 0)[3];
//	Mat leftres = left(Range(yl1, yl2), Range(xl1, xl2));
//	IplImage *rightipl = &IplImage(right);
//	int xr1 = getcutpos(rightipl, 0)[0];
//	int xr2 = getcutpos(rightipl, 0)[1];
//	int yr1 = getcutpos(rightipl, 0)[2];
//	int yr2 = getcutpos(rightipl, 0)[3];
//	Mat rightres = right(Range(yr1, yr2), Range(xr1, xr2));
//	bitwise_not(leftres, leftres);
//	bitwise_not(rightres, rightres);
//	sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//	imwrite(save_file, leftres);
//	numpic++;
//	sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//	imwrite(save_file, rightres);
//	numpic++;
//}
//else
//{
//	IplImage *a1ipl = &IplImage(matfsp);
//	int x1 = getcutpos(a1ipl, 0)[0];
//	int x2 = getcutpos(a1ipl, 0)[1];
//	int y1 = getcutpos(a1ipl, 0)[2];
//	int y2 = getcutpos(a1ipl, 0)[3];
//	Mat a1res = matfsp(Range(y1, y2), Range(x1, x2));
//	bitwise_not(a1res, a1res);
//	sprintf(save_file, "%s%d.jpg", Savedir, numpic);
//	imwrite(save_file, a1res);
//	numpic++;
//}