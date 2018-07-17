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

int main_vfm() {

	//string dir_path = "E:\\VS2010\\nbOCR\\Debug\\´ý·Ö¸îÍ¼Æ¬\\chi003.jpg";
	//Mat merimg;
	//
	//Mat img0 = imread("C:\\Users\\Administrator\\Desktop\\100027_0.png", IMREAD_GRAYSCALE);
	//Mat img1 = imread("C:\\Users\\Administrator\\Desktop\\100027_1.png", IMREAD_GRAYSCALE);
	//Mat img2 = imread("C:\\Users\\Administrator\\Desktop\\100027_2.png", IMREAD_GRAYSCALE);
	//Mat img3 = imread("C:\\Users\\Administrator\\Desktop\\100027_3.png", IMREAD_GRAYSCALE);
	//Mat img4 = imread("C:\\Users\\Administrator\\Desktop\\100027_4.png", IMREAD_GRAYSCALE);
	//Mat img5 = imread("C:\\Users\\Administrator\\Desktop\\100027_5.png", IMREAD_GRAYSCALE);
	//Mat img6 = imread("C:\\Users\\Administrator\\Desktop\\100027_6.png", IMREAD_GRAYSCALE);
	//Mat img7 = imread("C:\\Users\\Administrator\\Desktop\\100027_7.png", IMREAD_GRAYSCALE);
	//Mat img8 = imread("C:\\Users\\Administrator\\Desktop\\100027_8.png", IMREAD_GRAYSCALE);
	//vector<Mat> mer;
	//mer.push_back(img0);
	//mer.push_back(img1);
	//mer.push_back(img2);
	//mer.push_back(img3);
	//mer.push_back(img4);
	////mer.push_back(img5);
	////mer.push_back(img6);
	////mer.push_back(img7);
	////mer.push_back(img8);
	//merge(mer, merimg);
	//imshow("nn", merimg);
	////imwrite("C:\\Users\\Administrator\\Desktop\\he.png", merimg);

	Mat src = imread("C:\\Users\\Administrator\\Desktop\\Î¢ÐÅÍ¼Æ¬_20180615150955.png");
	
	cout << src.channels() << endl;
	vector<Mat> chan;
	split(src, chan);
	for (int i = 0; i < chan[2].rows; i++)
		for (int j = 0; j < chan[2].cols; j++) {
			cout << (int)chan[2].at<uchar>(i, j)<<" ";
		}
	waitKey(0);
	system("pause");
	return 0;
}