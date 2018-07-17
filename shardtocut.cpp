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

int main_vdbgnh() {

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
		fileNames = dir.GetListFiles(dirNames[i1], "*.png", false);
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

			Mat src1 = src(Range(src.rows * 0.2, src.rows * 0.88), Range(0, src.cols));
			float aspect = (float)src1.cols / src1.rows;
			cout << (float)src1.cols / src1.rows << endl;
			Mat dst;
			imshow("cds", src1);
			bilateralFilter(src1, dst, 40, 80, 20);
			//Mat dst1 = inhance(dst, 2.0);
			//imshow("m", dst1);

			Mat gray;
			vector<Vec4i> Lines;
			cvtColor(src1, gray, CV_BGR2GRAY);
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
			Mat red1 = splitTored(dst);
			Mat red = inhance(red1, 2.5);
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
			Mat deleLinebinaryImg = red_binary.clone();
			//������ֵ����ĺ������
			deleteLines(deleLinebinaryImg);
			Mat binaryLineImgCopy = deleLinebinaryImg.clone();
			//imshow("binary", binaryLineImgCopy);
			//ȥ��С����׵�
			RemoveSmallRegion(binaryLineImgCopy, binaryLineImgCopy, 8, 1, 1);
			//bool firstcut = firstwhite(binaryLineImgCopy);
			//vector<int> cutpointfromleft = getCutfromleftandright(binaryLineImgCopy);
			Mat cutred, cutred1;
			if ((aspect > 5.0) && (aspect < 6.0)) {
				if (src.rows == 115) {
					line(red, Point(binaryLineImgCopy.cols - 58, 0), Point(binaryLineImgCopy.cols - 58, binaryLineImgCopy.rows),
						Scalar(255, 255, 255), 2, 8);
					cutred = red(Range(binaryLineImgCopy.rows / 2, binaryLineImgCopy.rows), Range(binaryLineImgCopy.cols * 0.071, binaryLineImgCopy.cols));
					cutred1 = red1(Range(binaryLineImgCopy.rows / 2, binaryLineImgCopy.rows), Range(binaryLineImgCopy.cols * 0.071, binaryLineImgCopy.cols));
				}
				else
				{//ƴ������
					/*Mat tempup = red(Range(5, 43), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
					Mat tempup1 = red1(Range(5, 43), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
					Mat tempdown = red(Range(43, 81), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
					Mat tempdown1 = red1(Range(43, 81), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
					hconcat(tempup, tempdown, cutred);
					hconcat(tempup1, tempdown1, cutred1);*/
					//���Ÿ�tenssoract
					cutred = red(Range(0, red.rows), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
					cutred1 = red1(Range(0, red.rows), Range(binaryLineImgCopy.cols * 0.16, binaryLineImgCopy.cols));
				}
			}
			else if ((aspect > 8.0) && (aspect < 10.0)) {
				cutred = red;
				cutred1 = red1;
			}
			else if ((aspect > 12.0) && (aspect < 13.0)) {
				cutred = red(Range(0, binaryLineImgCopy.rows), Range(binaryLineImgCopy.cols * 0.287, binaryLineImgCopy.cols * 0.861));
				cutred1 = red1(Range(0, binaryLineImgCopy.rows), Range(binaryLineImgCopy.cols * 0.287, binaryLineImgCopy.cols * 0.861));
			}
			else if ((aspect > 10.0) && (aspect < 11.0)) {
				cutred = red(Range(0, binaryLineImgCopy.rows), Range(10, binaryLineImgCopy.cols * 0.396));
				cutred1 = red1(Range(0, binaryLineImgCopy.rows), Range(10, binaryLineImgCopy.cols * 0.396));
			}
			else {
				cutred = red;
				cutred1 = red1;
			}
			thres = otsu2(&IplImage(cutred.clone()));
			threshold(cutred, red_binary, thres, 255, CV_THRESH_BINARY_INV);
			imshow("v", red_binary);
			deleLinebinaryImg = red_binary.clone();
			deleteLines(deleLinebinaryImg);
			binaryLineImgCopy = deleLinebinaryImg.clone();
			imshow("binary", binaryLineImgCopy);
			RemoveSmallRegion(binaryLineImgCopy, binaryLineImgCopy, 8, 1, 1);
			imshow("vu", binaryLineImgCopy);
			//imwrite("C:\\Users\\Administrator\\Desktop\\after4.jpg", binaryLineImgCopy);
			//Ѱ����ͨ��������contours���洢���������ĵ�
			vector<vector<Point>> contours;
			findContours(binaryLineImgCopy,
				contours,   //����������  
							CV_RETR_EXTERNAL,
				//CV_RETR_TREE,   //��ȡ��������  
				CV_CHAIN_APPROX_NONE);
			//�Ƴ���������̵�����  
			removeShortContours(contours, 0, 500);
			Mat contoursImg = Mat(deleLinebinaryImg.rows, deleLinebinaryImg.cols, CV_8U);
			drawContours(contoursImg, contours);
			imshow("contoursImg", contoursImg);
			//����ͨ��ת��Ϊ�����ʾ����x1��������
			vector<int*> coordinates = CCPoints2Coordinates(contours);
			sort(coordinates.begin(), coordinates.end(), coordinateCmp);
			/*Mat ccaImg = cutred.clone();
			for (int i = 0; i < coordinates.size(); i++) {
				rectangle(ccaImg, Point(coordinates.at(i)[0], coordinates.at(i)[2]), Point(coordinates.at(i)[1], coordinates.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			imshow("cca", ccaImg);*/
			vector<int*> combined = combineOverlapComponents(coordinates);
			adjustCombinedComponent(combined);
			/*Mat combinedImg = cutred.clone();
			for (int i = 0; i < combined.size(); i++) {
				rectangle(combinedImg, Point(combined.at(i)[0], combined.at(i)[2]), Point(combined.at(i)[1], combined.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			imshow("combine", combinedImg);*/
			//�����и�
			int LH = estimateComponentsMean(combined);
			//Ǳ��ճ��ģʽ�����ص�ָ��ָ�򳤶�Ϊ������飬ǰ�ĸ�λ�ô洢���꣬�����λ����Ǳ��ģʽ��combined�е�index
			vector<int*> potentialPT = detectPotentialPT(combined, LH, _thetah1, _thetah2);
			//̽�ⵥ�ʻ�����
			vector<vector<int*>> singleStrokes = detectSingleStroke(binaryLineImgCopy, potentialPT, LH * 0.25, 1);
			/*Mat singleStrokeImg = cutred.clone();
			for (int i = 0; i < singleStrokes.size(); i++) {
				for (int j = 0; j < singleStrokes.at(i).size(); j++) {
					rectangle(singleStrokeImg, Point(singleStrokes.at(i).at(j)[0], singleStrokes.at(i).at(j)[2]),
						Point(singleStrokes.at(i).at(j)[1], singleStrokes.at(i).at(j)[3]), Scalar(0, 0, 0), 0.2);
				}
			}
			imshow("single_strokes", singleStrokeImg);*/
			//ʹ�õ��ʻ������һ������
			vector<int*> split = splitBySingleStroke(combined, singleStrokes);
			Mat splitImg = cutred.clone();
			for (int i = 0; i < split.size(); i++) {
				rectangle(splitImg, Point(split.at(i)[0], split.at(i)[2]), Point(split.at(i)[1], split.at(i)[3]), Scalar(0, 0, 0), 0.2);
			}
			imshow("split", splitImg);
			waitKey(0);

			Mat fgImg = cutred1.clone();
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

