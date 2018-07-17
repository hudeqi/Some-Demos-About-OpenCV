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
int center_r[N];
int summ_r[10000];

int main_cbis() {

	char savedir[200];
	setPathOfexe(savedir);
	string dir_path(savedir);
	//dir_path += "���ָ�ͼƬ\\";
	//Directory dir;
	string dirpath = dir_path + "���ָ�ͼƬ";
	vector<string> dirNames;
	getAllFiles(dirpath, dirNames);
	//vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);
	int countpic = 0;
	for (int i1 = 0; i1 < dirNames.size(); i1++)
	{
		vector<string> fileNames;
		Directory dir;
		fileNames = dir.GetListFiles(dirNames[i1], "*.jpg", false);
		for (int numFile = 0; numFile < fileNames.size(); numFile++) {
			char Savedir[200];
			strcpy(Savedir, savedir);
			string fileName = fileNames[numFile];
			string fileFullName = dirNames[i1] + "\\" + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl << endl;

			//Mat src = imread("C:\\Users\\Administrator\\Desktop\\ת��ǰ��\\after.jpg");
			//Mat src = imread("E:\\VS2010\\nbOCR\\Debug\\���ָ�ͼƬ\\0001 (3).jpg");
			Mat src = imread(fileFullName);
			Mat dst;
			imshow("cds", src);
			bilateralFilter(src, dst, 40, 80, 20);
			//dst = inhance(dst, 2.0);
			imshow("m", dst);

			Mat gray;
			vector<Vec4i> Lines;
			cvtColor(src, gray, CV_BGR2GRAY);
			//���ߵ����������������ж�Ʊ������
			Lines = drawLinesforPic(gray, 400);
			/*for (int i = 0; i < Lines.size(); i++)
			{
				line(gray, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(255, 0, 0), 2, 8);
			}*/
			//imshow("nn", gray);

			bool flag = true;
			if (Lines.size() >= 15)
				flag = false;
			int th = 90;
			int thres;
			//�������º�ɫͨ��
			Mat red = splitTored(dst);
			imshow("kks", red);
			//imwrite("C:\\Users\\Administrator\\Desktop\\bjbd.jpg", red);
			//red = ACE(red, 3, 3, 10.0);
			Mat red_binary;
			//imshow("cc", red);
			thres = otsu2(&IplImage(red.clone()));
			if (flag)
				threshold(red, red_binary, thres, 255, CV_THRESH_BINARY_INV);
			else
			{
				threshold(red, red_binary, th, 255, CV_THRESH_BINARY_INV);
			}
			imshow("v", red_binary);
			Mat deleLinebinaryImg = red_binary.clone();
			//������ֵ����ĺ������
			deleteLines(deleLinebinaryImg);
			Mat binaryLineImgCopy = deleLinebinaryImg.clone();
			//imshow("binary", binaryLineImgCopy);
			//ȥ��С����׵�
			RemoveSmallRegion(binaryLineImgCopy, binaryLineImgCopy, 4, 1, 1);
			imshow("vu", binaryLineImgCopy);
			//imwrite("C:\\Users\\Administrator\\Desktop\\after4.jpg", binaryLineImgCopy);
			//Ѱ����ͨ��������contours���洢���������ĵ�
			vector<vector<Point>> contours;
			findContours(binaryLineImgCopy,
				contours,   //����������  
							//CV_RETR_EXTERNAL,
				CV_RETR_TREE,   //��ȡ��������  
				CV_CHAIN_APPROX_NONE);
			//�Ƴ���������̵�����  
			removeShortContours(contours, 2, 500);
			Mat contoursImg = Mat(deleLinebinaryImg.rows, deleLinebinaryImg.cols, CV_8U);
			drawContours(contoursImg, contours);
			imshow("contoursImg", contoursImg);
			//����ͨ��ת��Ϊ�����ʾ����x1��������
			vector<int*> coordinates = CCPoints2Coordinates(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			Mat ccaImg = red.clone();
			for (int i = 0; i < coordinates.size(); i++) {
				rectangle(ccaImg, Point(coordinates.at(i)[0], coordinates.at(i)[2]), Point(coordinates.at(i)[1], coordinates.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			//imshow("cca", ccaImg);
			Mat combinedImg = red.clone();
			vector<int*> combined = combineOverlapComponents(coordinates);
			adjustCombinedComponent(combined);
			for (int i = 0; i < combined.size(); i++) {
				rectangle(combinedImg, Point(combined.at(i)[0], combined.at(i)[2]), Point(combined.at(i)[1], combined.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			//imshow("combine", combinedImg);
			//�����и�
			int LH = estimateComponentsMean(combined);
			//Ǳ��ճ��ģʽ�����ص�ָ��ָ�򳤶�Ϊ������飬ǰ�ĸ�λ�ô洢���꣬�����λ����Ǳ��ģʽ��combined�е�index
			vector<int*> potentialPT = detectPotentialPT(combined, LH, _thetah1, _thetah2);
			//̽�ⵥ�ʻ�����
			vector<vector<int*>> singleStrokes = detectSingleStroke(deleLinebinaryImg, potentialPT, LH * 0.25, 1);
			Mat singleStrokeImg = red.clone();
			for (int i = 0; i < singleStrokes.size(); i++) {
				for (int j = 0; j < singleStrokes.at(i).size(); j++) {
					rectangle(singleStrokeImg, Point(singleStrokes.at(i).at(j)[0], singleStrokes.at(i).at(j)[2]),
						Point(singleStrokes.at(i).at(j)[1], singleStrokes.at(i).at(j)[3]), Scalar(0, 0, 0), 0.2);
				}
			}
			//imshow("single_strokes", singleStrokeImg);
			//ʹ�õ��ʻ������һ������
			vector<int*> split = splitBySingleStroke(combined, singleStrokes);
			Mat splitImg = red.clone();
			for (int i = 0; i < split.size(); i++) {
				rectangle(splitImg, Point(split.at(i)[0], split.at(i)[2]), Point(split.at(i)[1], split.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			imshow("split", splitImg);
			waitKey(0);

			Mat fgImg = red.clone();
			vector<int> cutpoint;
			cutpoint = getcutPoint(split, splitImg);
			//savePicture(cutpoint, fileName, Savedir, fgImg);
			saverecPic(split, fileName, Savedir, fgImg);
			countpic++;
			cout << countpic << endl;
		}
	}
	return 0;
}

