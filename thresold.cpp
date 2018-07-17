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

int otsu3(IplImage *image)
{
	int w = image->width;
	int h = image->height;

	unsigned char*np; // 图像指针  
	unsigned char pixel;
	int thresholdValue = 1; // 阈值  
	int ihist[256]; // 图像直方图，256个点  

	int i, j, k; // various counters  
	int n, n1, n2, gmin, gmax;
	double m1, m2, sum, csum, fmax, sb;

	// 对直方图置零...  
	memset(ihist, 0, sizeof(ihist));

	gmin = 255; gmax = 0;
	// 生成直方图  
	for (i = 0; i < h; i++)
	{
		np = (unsigned char*)(image->imageData + image->widthStep*i);
		for (j = 0; j < w; j++)
		{
			pixel = np[j];
			ihist[pixel]++;
			if (pixel > gmax) gmax = pixel;
			if (pixel < gmin) gmin = pixel;
		}
	}

	// set up everything  
	sum = csum = 0.0;
	n = 0;

	for (k = 0; k <= 255; k++)
	{
		sum += k * ihist[k]; /* x*f(x) 质量矩*/
		n += ihist[k]; /* f(x) 质量 */
	}

	if (!n)
	{
		// if n has no value, there is problems...  
		//fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");  
		thresholdValue = 160;
		goto L;
	}

	// do the otsu global thresholding method  
	fmax = -1.0;
	n1 = 0;
	for (k = 0; k <255; k++)
	{
		n1 += ihist[k];
		if (!n1) { continue; }
		n2 = n - n1;
		if (n2 == 0) { break; }
		csum += k *ihist[k];
		m1 = csum / n1;
		m2 = (sum - csum) / n2;
		sb = n1 * n2 *(m1 - m2) * (m1 - m2);
		/* bbg: note: can be optimized. */
		if (sb > fmax)
		{
			fmax = sb;
			thresholdValue = k;
		}
	}

L:
	for (i = 0; i < h; i++)
	{
		np = (unsigned char*)(image->imageData + image->widthStep*i);
		for (j = 0; j < w; j++)
		{
			if (np[j] >= thresholdValue)
				np[j] = 255;
			else np[j] = 0;
		}
	}

	//cout<<"The Threshold of this Image in Otsu is:"<<thresholdValue<<endl;  
	return(thresholdValue);
}


//一堆有目录名的黑底白字图进行二值化后再行成相同目录名存进去
int main_bfgn() {
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
			//string fileFullName = dir_path + fileName;
			cout << "file name:" << fileName << endl;
			//cout << "file paht:" << fileFullName << endl << endl;

			Mat img1;
			Mat img0 = imread(fileName);
			cvtColor(img0, img1, CV_RGB2GRAY);
			IplImage *roi2 = &IplImage(img1);
			int thres = otsu3(roi2);
			cout << thres << endl;
			int width = roi2->width;
			int height = roi2->height;
			uchar *ptr = (uchar *)roi2->imageData;
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if ((int)ptr[i*roi2->widthStep + j] >= thres)
						ptr[i*roi2->widthStep + j] = 255;
					else
					{
						ptr[i*roi2->widthStep + j] = 0;
					}
				}
			}
			//cvThreshold(roi2, roi2, thres, 255, CV_THRESH_BINARY);
			//bitwise_not(roi2, roi2);
			char *filename = (char*)fileName.data();
			char *p = strrchr(filename, '\\');
			string ps(p);
			string nn = ps.substr(1, ps.length() - 4) + "png";
			int pslength = ps.length();
			fileName.erase(fileName.length() - pslength, pslength);
			char *filename1 = (char*)fileName.data();
			char *p1 = strrchr(filename1, '\\');
			string dir1(p1);
			string dir = dir1.substr(1, dir1.length());
			string dir2 = "分割\\" + dir + "\\";
			char *S = (char*)dir2.data();
			strcat(Savedir, S);
			FILE *fp = NULL;
			fp = fopen(Savedir, "w");
			if (!fp)
				mkdir(Savedir);
			char *SS = (char*)nn.data();
			strcat(Savedir, SS);
			Mat bb = Mat(roi2);
			RemoveSmallRegion(bb, bb, 10, 1, 1);
			imshow("vvacd", bb);
			bitwise_not(bb, bb);
			cvtColor(bb, bb, CV_GRAY2BGR);
			Mat thinresult = getthinImage(bb);
			thinresult = thinresult * 255;
			imshow("vva", thinresult);
			Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
			morphologyEx(thinresult, thinresult, MORPH_DILATE, element);
			imshow("vvvfa", thinresult);
			bitwise_not(thinresult, thinresult);
			//cvSaveImage(Savedir, roi2);
			imwrite(Savedir, thinresult);
			waitKey();
		}
		
		
	}
	return 0;
}


int main_cfvfdb() {
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\微信图片_20180627092808.png", 0);
	threshold(src, src, 0, 255, CV_THRESH_OTSU);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++) {
			if ((int)src.at<uchar>(i, j) != 0 && (int)src.at<uchar>(i, j) != 255)
				cout << (int)src.at<uchar>(i, j) << "  ";
		}
	system("pause");
	return 0;
}