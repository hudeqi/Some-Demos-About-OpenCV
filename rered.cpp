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
#include "strngs.h"  
#include "baseapi.h"
#include "time.h"
#include "renderer.h"
#include <allheaders.h>

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h> 
using namespace std;
using namespace cv;




int main()
{
	char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	string dir_path(savedir);
	dir_path += "´ý·Ö¸îÍ¼Æ¬\\";
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		char Savedir[200];
		strcpy(Savedir, savedir);
		string fileName = fileNames[ii];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl << endl;
		CvMat *inputImage;
		CvMat *	outputImage;
		int i, j;
		char*fullName = (char*)fileFullName.data();
		inputImage = cvLoadImageM(fullName, 1);
		IplImage* image = cvCreateImage(cvGetSize(inputImage), 8, 3);
		cvGetImage(inputImage, image);
		IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);
		cvCvtColor(image, hsv, CV_BGR2HSV);
		int width = hsv->width;
		int height = hsv->height;
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++)
			{
				CvScalar s_hsv = cvGet2D(hsv, i, j);
				CvScalar s;
				if (!(((s_hsv.val[0]>0) && (s_hsv.val[0] < 12))))
				{
					s.val[0] = 0;
					s.val[1] = 0;
					s.val[2] = 0;
					cvSet2D(hsv, i, j, s);
				}
			}
		outputImage = cvCreateMat(hsv->height, hsv->width, CV_8UC3);
		cvConvert(hsv, outputImage);
		Mat m(hsv);
		Mat mgray;
		Mat source(inputImage);
		cv::cvtColor(m, mgray, CV_BGR2GRAY);
		tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
		api->Init(NULL, "chi_sim");
		api->SetImage((uchar*)mgray.data, mgray.cols, mgray.rows, 1, mgray.cols);
		Boxa* boxes = api->GetComponentImages(tesseract::RIL_BLOCK, false, NULL, NULL);
		string dir(savedir);
		dir += "·Ö¸î\\";
		char*cdir = (char*)dir.data();
		char saveFile[200];
		for (int i = 0; i < boxes->n; i++) {
			BOX* box = boxaGetBox(boxes, i, L_CLONE);
			api->SetRectangle(box->x, box->y, box->w, box->h);
			Mat roi = source(Range(box->y, box->h + box->y + 1), Range(box->x, box->x + box->w + 1));
			sprintf(saveFile, "%s%s_%s.jpg", cdir, fileName, i);
			imwrite(saveFile, roi);
		}
	}
	return 0;
}