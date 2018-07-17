//#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp>  
//#include <math.h>  
//#include <string.h>  
//#include <opencv/cv.h>  
//#include <stdio.h>
//#include <iostream>
//#include<fstream>
//#include<iomanip>
//#include "guidedFilter.h"
//#include "KMeans.h"
//#include "CCA.h"
//#include <direct.h>
//#include <windows.h>
//#include <tchar.h>
//#include "opencv2\contrib\contrib.hpp"
//
//using namespace std;
//using namespace cv;
//#define N 1000
//#define MAXSIZE 5000
//#define _thetah1 1.0
//#define _thetah2 1.0
//int center[N];
//int summ[10000];
//
//Mat norm(const Mat& src) {
//	// Create and return normalized image:  
//	Mat dst;
//	switch (src.channels()) {
//	case 1:
//		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
//		break;
//	case 3:
//		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
//		break;
//	default:
//		src.copyTo(dst);
//		break;
//	}
//	return dst;
//}
//
//Mat getroi(Mat img0, int up, int down) {
//	Mat img1, X, I, CannyImg, DstImg, roi0, roi1, roi2;
//	int K;
//	img1 = img0;
//	img1.convertTo(X, CV_32FC1); //gamaУ��
//	float gamma = 2.0;
//	pow(X, gamma, I);
//	imshow("Gamma correction image", norm(I));
//	Canny(norm(I), CannyImg, 140, 250, 3);
//	//imshow("CannyImg", CannyImg);
//	cvtColor(img0, DstImg, CV_GRAY2BGR);
//	vector<Vec4i> Lines;
//	HoughLinesP(CannyImg, Lines, 1, CV_PI / 360, 170, 30, 15);
//	int *tempy = new int[Lines.size()];
//	for (size_t i = 0; i < Lines.size(); i++)
//	{
//		//line(DstImg, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(0, 0, 255), 2, 8);
//		double a = 0.0;
//		double b = 0.0;
//		a = (Lines[i][3] - Lines[i][1]) / (Lines[i][2] - Lines[i][0]);
//		b = Lines[i][3] - a * Lines[i][2];
//		tempy[i] = a * DstImg.cols / 2 + b;
//	}
//	/*imshow("HoughLines_Detect", DstImg);
//	waitKey(0);*/
//	//imwrite("HoughLines_Detect.jpg", DstImg);
//	int Num = Lines.size();
//	int maxy = -1;
//	int miny = 9999;
//	if (Num >= 2) {
//		for (int i = 0; i < Num; i++) {
//			if (tempy[i] > maxy)
//				maxy = tempy[i];
//			if (tempy[i] < miny)
//				miny = tempy[i];
//		}
//	}
//	int diff = maxy - miny;
//	if (Num == 0)
//		K = 0;
//	if (Num == 1)
//		K = 1;
//	if (Num >= 2) {
//		if (diff <= 10)
//			K = 1;
//		else if ((diff > 10) && (diff <= 70))
//			K = 2;
//		else
//			K = 3;
//	}
//	if ((K == 2) || (K == 3)) {
//		float *means = new float[K];
//		cluster_p(Num, tempy, K, center, means);
//	}
//	int clusters3[3] = { 0,0,0 };
//	int count3[3] = { 0,0,0 };
//	int clusters2[2] = { 0,0 };
//	int count2[2] = { 0,0 };
//	if (K == 3) {
//		for (int i = 0; i<Lines.size(); i++) {
//			clusters3[center[i]] += tempy[i];
//			count3[center[i]] += 1;
//		}
//		for (int i = 0; i<3; i++) {
//			clusters3[i] /= count3[i];
//		}
//	}
//	if (K == 2) {
//		for (int i = 0; i<Lines.size(); i++) {
//			clusters2[center[i]] += tempy[i];
//			count2[center[i]] += 1;
//		}
//		for (int i = 0; i<2; i++) {
//			clusters2[i] /= count2[i];
//		}
//	}
//	int minIndex = -1;
//	int maxIndex = -1;
//	if (K == 2)
//	{
//		int indexes[2] = { 0,0 };
//		for (int i = 0; i<2; i++)
//		{
//			int tempMaxValue = -1;
//			int tempMaxIndex = -1;
//			for (int j = 0; j<2; j++)
//			{
//				if (clusters2[j] > tempMaxValue)
//				{
//					tempMaxValue = clusters2[j];
//					tempMaxIndex = j;
//				}
//			}
//			indexes[i] = tempMaxIndex;
//			clusters2[tempMaxIndex] = 0;
//		}
//		maxIndex = indexes[0];
//		minIndex = indexes[1];
//	}
//	if (K == 3)
//	{
//		int indexes[2] = { 0,0 };
//		for (int i = 0; i<2; i++)
//		{
//			int tempMaxValue = -1;
//			int tempMaxIndex = -1;
//			for (int j = 0; j<3; j++)
//			{
//				if (clusters3[j] > tempMaxValue)
//				{
//					tempMaxValue = clusters3[j];
//					tempMaxIndex = j;
//				}
//			}
//			indexes[i] = tempMaxIndex;
//			clusters3[tempMaxIndex] = 0;
//		}
//		maxIndex = indexes[0];
//		minIndex = indexes[1];
//	}
//
//	int finalMaxIndex = -1;
//	int finalMinIndex = -1;
//	int finalMaxValue = -1;
//	int finalMinValue = 9999;
//	for (int i = 0; i<Lines.size(); i++) {
//		if (center[i] == minIndex)
//		{
//			if (tempy[i]>finalMaxValue)
//			{
//				finalMaxValue = tempy[i];
//				finalMaxIndex = i;
//			}
//		}
//
//		if (center[i] == maxIndex)
//		{
//			if (tempy[i]<finalMinValue)
//			{
//				finalMinValue = tempy[i];
//				finalMinIndex = i;
//			}
//		}
//
//	}
//
//	//imshow("HoughLines_Detect", DstImg);
//	//imwrite("HoughLines_Detect.jpg", DstImg);
//	/*outFile.getline(buffer, 256, '\n');
//	string s3(buffer);
//	thres = static_cast<int>(strtol(s3.c_str(), &end, 10));*/
//	if ((K == 2) || (K == 3)) {
//		roi0 = DstImg(Range(0, tempy[finalMaxIndex] + up), Range(0, DstImg.cols));
//		roi1 = DstImg(Range(tempy[finalMaxIndex] + up, tempy[finalMinIndex] + down), Range(0, DstImg.cols));
//		roi2 = DstImg(Range(tempy[finalMinIndex] + down, DstImg.rows), Range(0, DstImg.cols));
//	}
//	if ((K == 0) || (K == 1))
//	{
//		roi1 = DstImg;
//	}
//	return roi1;
//}
//
////ʹ��singlestroke�ָ�combined
//vector<int*> splitBySingleStroke(vector<int*> combined, vector<vector<int*>> singleStrokes) {
//	if (singleStrokes.size() == 0) {
//		return combined;
//	}
//	vector<int*> split;
//	//singleStrokeIndex
//	int ssi = 0;
//	for (int i = 0; i < combined.size(); i++) {
//		//singleStrokeIndexInCombined
//		int ssiinc;
//		if (ssi < singleStrokes.size()) {
//			ssiinc = singleStrokes.at(ssi).at(0)[4];
//		}
//		else {
//			ssiinc = -1;
//		}
//		if (i != ssiinc) {
//			int* p = (int*)malloc(4 * sizeof(int));
//			p[0] = combined.at(i)[0];
//			p[1] = combined.at(i)[1];
//			p[2] = combined.at(i)[2];
//			p[3] = combined.at(i)[3];
//			split.push_back(p);
//		}
//		else {
//			int areal = combined.at(i)[0];
//			int arear = combined.at(i)[1];
//			int areau = combined.at(i)[2];
//			int areab = combined.at(i)[3];
//			int splitl, splitr;
//			for (int j = 0; j < singleStrokes.at(ssi).size(); j++) {
//				if (j == 0) {
//					splitl = areal;
//				}
//				else {
//					splitl = (singleStrokes.at(ssi).at(j - 1)[0] + singleStrokes.at(ssi).at(j - 1)[1]) / 2;
//				}
//				splitr = (singleStrokes.at(ssi).at(j)[0] + singleStrokes.at(ssi).at(j)[1]) / 2;
//				int* p = (int*)malloc(4 * sizeof(int));
//				p[0] = splitl;
//				p[1] = splitr;
//				p[2] = areau;
//				p[3] = areab;
//				split.push_back(p);
//			}
//			splitl = (singleStrokes.at(ssi).at(singleStrokes.at(ssi).size() - 1)[0] +
//				singleStrokes.at(ssi).at(singleStrokes.at(ssi).size() - 1)[1]) / 2;
//			splitr = arear;
//			int* p = (int*)malloc(4 * sizeof(int));
//			p[0] = splitl;
//			p[1] = splitr;
//			p[2] = areau;
//			p[3] = areab;
//			split.push_back(p);
//			ssi++;
//		}//end if
//	}//end for i
//	return split;
//}
//
////ֻ�е��ʻ��߶ȵ��ڻ����heightThreshold����ȸ��ڵ���widthThreshold�ű���Ϊ�ǵ��ʻ�����
//vector<vector<int*>> detectSingleStroke(Mat binaryLineImg, vector<int*> potentialPT, int heightThreshold, int widthThreshold) {
//	vector<vector<int*>> singleStrokesForAll;
//	for (int i = 0; i < potentialPT.size(); i++) {
//		vector<int*> singleStrokes;
//		int* p = potentialPT.at(i);
//		int l = p[0], r = p[1], u = p[2], b = p[3], index = p[4];
//		//1:single, 0:multi
//		int* single = (int*)malloc((r - l + 1) * sizeof(int));
//		bool singleStroke;
//		//����single��
//		vector<int> singleArea;
//		//��
//		for (int j = l; j <= r; j++) {
//			singleStroke = TRUE;
//			//�ʻ���ʼ
//			bool start = FALSE;
//			int startPos = 0;
//			//�ʻ�����
//			bool finish = FALSE;
//			for (int k = u; k <= b; k++) {
//				uchar *p = (binaryLineImg.ptr<uchar>(k) + j);
//				//cout << (int)*p << ",";
//			}
//			//��
//			for (int k = u; k <= b; k++) {
//				uchar *p = (binaryLineImg.ptr<uchar>(k) + j);
//				int tmp = *p;
//				//��ɫ255
//				if (*p == 255 && start == FALSE) {
//					start = TRUE;
//					startPos = k;
//					continue;
//				}
//				if (*p == 0 && start == TRUE && finish == FALSE) {
//					finish = TRUE;
//					if ((k - startPos) > heightThreshold) {
//						singleStroke = FALSE;
//						break;
//					}
//					continue;
//				}
//				if (*p == 255 && finish == TRUE) {
//					singleStroke = FALSE;
//					break;
//				}
//			}//end for k
//			if (singleStroke)
//				singleArea.push_back(j);
//		}//end for j
//		int start;
//		if (singleArea.size() == 0) {
//			continue;
//		}
//		//ת����ʾ
//		for (int j = 0; j < singleArea.size(); j++) {
//			if (j == 0) {
//				start = j;
//				continue;
//			}
//			if (j == singleArea.size() - 1) {
//				if (singleArea.at(j) == (singleArea.at(j - 1) + 1)) {
//					int* oneArea = (int*)malloc(5 * sizeof(int));
//					oneArea[0] = singleArea.at(start);
//					oneArea[1] = singleArea.at(j);
//					oneArea[4] = index;
//					oneArea[2] = u;
//					oneArea[3] = b;
//					if (oneArea[1] - oneArea[0] >= widthThreshold)
//						singleStrokes.push_back(oneArea);
//				}
//				else {
//					int* oneArea = (int*)malloc(5 * sizeof(int));
//					oneArea[0] = singleArea.at(j);
//					oneArea[1] = singleArea.at(j);
//					oneArea[4] = index;
//					oneArea[2] = u;
//					oneArea[3] = b;
//					if (oneArea[1] - oneArea[0] >= widthThreshold)
//						singleStrokes.push_back(oneArea);
//				}
//			}
//			if (singleArea.at(j) != (singleArea.at(j - 1) + 1)) {
//				//��һ�ڶ���������ʼ�㣬��ֹ�㣬�������ĸ������½硣���������stroke���ڵ�������combined�е�index
//				int* oneArea = (int*)malloc(5 * sizeof(int));
//				oneArea[0] = singleArea.at(start);
//				oneArea[1] = singleArea.at(j - 1);
//				oneArea[4] = index;
//				oneArea[2] = u;
//				oneArea[3] = b;
//				start = j;
//				if (oneArea[1] - oneArea[0] >= widthThreshold)
//					singleStrokes.push_back(oneArea);
//			}
//		}//end for j
//		if (singleStrokes.size() > 0)
//			singleStrokesForAll.push_back(singleStrokes);
//	}//end for i
//	return singleStrokesForAll;
//}
//
//vector<int*> detectPotentialPT(vector<int*> combinedCC, int LH, float thetah1, float thetah2) {
//	vector<int*> potentialPT;
//	for (int i = 0; i < combinedCC.size(); i++) {
//		float sw = combinedCC.at(i)[1] - combinedCC.at(i)[0];
//		float sh = combinedCC.at(i)[3] - combinedCC.at(i)[2];
//		if (sw / sh > thetah1 || sw / LH > thetah2) {
//			int* coordinate = (int*)malloc(5 * sizeof(int));
//			coordinate[0] = combinedCC.at(i)[0];
//			coordinate[1] = combinedCC.at(i)[1];
//			coordinate[2] = combinedCC.at(i)[2];
//			coordinate[3] = combinedCC.at(i)[3];
//			coordinate[4] = i;
//			potentialPT.push_back(coordinate);
//		}
//	}
//	return potentialPT;
//}
//
//int estimateComponentsMean(vector<int*> combinedCC) {
//	map<int, int> hist;
//	map<int, int>::iterator iter;
//	if (combinedCC.size() == 1) {
//		return combinedCC.at(0)[3] - combinedCC.at(0)[2];
//	}
//	for (int i = 0; i < combinedCC.size() - 1; i++) {
//		int height = ((combinedCC.at(i)[3] - combinedCC.at(i)[2]) >(combinedCC.at(i + 1)[3] - combinedCC.at(i + 1)[2])) ?
//			(combinedCC.at(i)[3] - combinedCC.at(i)[2]) : (combinedCC.at(i + 1)[3] - combinedCC.at(i + 1)[2]);
//		int width = combinedCC.at(i + 1)[1] - combinedCC.at(i)[0];
//		if (hist.find(height) != hist.end()) {
//			hist[height] = hist[height] + width;
//		}
//		else
//		{
//			hist[height] = width;
//		}
//	}
//	iter = hist.begin();
//	int widthCount = 0;
//	int areaCount = 0;
//	while (iter != hist.end()) {
//		widthCount += iter->second;
//		areaCount += (iter->first * iter->second);
//		iter++;
//	}
//	return areaCount / widthCount;
//}
//
////ʹ����ͨ��û���ص�
//void adjustCombinedComponent(vector<int*> cc) {
//	for (int i = 0; i < cc.size() - 1; i++) {
//		if (cc.at(i)[1] > cc.at(i + 1)[0]) {
//			cc.at(i)[1] = (cc.at(i)[1] + cc.at(i + 1)[0]) / 2;
//			cc.at(i + 1)[0] = (cc.at(i)[1] + cc.at(i + 1)[0]) / 2 + 1;
//		}
//	}
//}
//
////����Ӧ�����������е�
//vector<int*> combineOverlapComponents(vector<int*> cc) {
//	vector<int*> combined;
//	int start = 0;
//	int curxl = cc.at(0)[0], curxr = cc.at(0)[1], curyu = cc.at(0)[2], curyb = cc.at(0)[3];
//	for (int i = 0; i < cc.size(); i++) {
//		if (i == cc.size() - 1) {
//			int* coordinate = (int*)malloc(4 * sizeof(int));
//			coordinate[0] = curxl;
//			coordinate[1] = curxr;
//			coordinate[2] = curyu;
//			coordinate[3] = curyb;
//			combined.push_back(coordinate);
//			break;
//		}
//		//i+1���Ƿ�Ҫ��i����
//		bool combine;
//		int x2l = cc.at(i + 1)[0];
//		int x2r = cc.at(i + 1)[1];
//		int y2u = cc.at(i + 1)[2];
//		int y2b = cc.at(i + 1)[3];
//		if (curxr > x2l) {
//			float ovlp = curxr - x2l;
//			float span = max(curxr, x2r) - curxl;
//			float dist = abs((x2l + x2r) / 2 - (curxl + curxr) / 2);
//			float w1 = curxr - curxl;
//			float w2 = x2r - x2l;
//			float nmovlp = (ovlp / w1 + ovlp / w2) / 2 - dist / span;
//			//cout << i << ":" << nmovlp << "\n";
//			if (nmovlp > 0.0) {
//				combine = TRUE;
//			}
//			else {
//				combine = FALSE;
//			}
//		}
//		else {
//			combine = FALSE;
//		}
//		if (combine) {
//			curxl = min(x2l, curxl);
//			curxr = max(x2r, curxr);
//			curyu = min(y2u, curyu);
//			curyb = max(y2b, curyb);
//		}
//		else {
//			int* coordinate = (int*)malloc(4 * sizeof(int));
//			coordinate[0] = curxl;
//			coordinate[1] = curxr;
//			coordinate[2] = curyu;
//			coordinate[3] = curyb;
//			combined.push_back(coordinate);
//			curxl = x2l;
//			curxr = x2r;
//			curyu = y2u;
//			curyb = y2b;
//		}
//	}
//	return combined;
//}
//
//bool coordinateCmp(int *p, int *q) {
//	return p[0]<q[0];
//}
//
//vector<int*> CCPoints2Coordinates(vector<vector<Point>> cc) {
//	vector<int*> coordinates;
//	for (int i = 0; i < cc.size(); i++) {
//		int x1, x2 = 0, y1, y2 = 0;
//		x1 = MAXSIZE;
//		y1 = MAXSIZE;
//		int numPoint = cc[i].size();
//		for (int j = 0; j < numPoint; j++) {
//			if (cc[i][j].x < x1) {
//				x1 = cc[i][j].x;
//			}
//			if (cc[i][j].x > x2) {
//				x2 = cc[i][j].x;
//			}
//			if (cc[i][j].y < y1) {
//				y1 = cc[i][j].y;
//			}
//			if (cc[i][j].y > y2) {
//				y2 = cc[i][j].y;
//			}
//		}
//		int* coordinate = (int*)malloc(4 * sizeof(int));
//		coordinate[0] = x1;
//		coordinate[1] = x2;
//		coordinate[2] = y1;
//		coordinate[3] = y2;
//		coordinates.push_back(coordinate);
//	}
//	return coordinates;
//}
//
//void drawContours(Mat img, vector<vector<Point>>contours) {
//	uchar color = 0;
//	for (int i = 0; i < contours.size(); i++) {
//		color = color > 150 ? 0 : color + 30;
//		//cout << (int)color << "!";
//		for (int j = 0; j < contours[i].size(); j++) {
//			Point point = contours[i][j];
//			int px = point.x;
//			int py = point.y;
//			uchar *p = (img.ptr<uchar>(py) + px);
//			*p = color;
//		}
//	}
//}
//
//void removeShortContours(vector<vector<Point>>& contours, int cmin, int cmax) {
//	vector<vector<Point>>::const_iterator itc = contours.begin();
//	while (itc != contours.end())
//	{
//		if (itc->size() < cmin || itc->size() > cmax)
//			itc = contours.erase(itc);
//		else
//			++itc;
//	}
//
//}
//
//
//int otsu2(IplImage *image)
//{
//	int w = image->width;
//	int h = image->height;
//
//	unsigned char*np; // ͼ��ָ��  
//	unsigned char pixel;
//	int thresholdValue = 1; // ��ֵ  
//	int ihist[256]; // ͼ��ֱ��ͼ��256����  
//
//	int i, j, k; // various counters  
//	int n, n1, n2, gmin, gmax;
//	double m1, m2, sum, csum, fmax, sb;
//
//	// ��ֱ��ͼ����...  
//	memset(ihist, 0, sizeof(ihist));
//
//	gmin = 255; gmax = 0;
//	// ����ֱ��ͼ  
//	for (i = 0; i < h; i++)
//	{
//		np = (unsigned char*)(image->imageData + image->widthStep*i);
//		for (j = 0; j < w; j++)
//		{
//			pixel = np[j];
//			ihist[pixel]++;
//			if (pixel > gmax) gmax = pixel;
//			if (pixel < gmin) gmin = pixel;
//		}
//	}
//
//	// set up everything  
//	sum = csum = 0.0;
//	n = 0;
//
//	for (k = 0; k <= 255; k++)
//	{
//		sum += k * ihist[k]; /* x*f(x) ������*/
//		n += ihist[k]; /* f(x) ���� */
//	}
//
//	if (!n)
//	{
//		// if n has no value, there is problems...  
//		//fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");  
//		thresholdValue = 160;
//		goto L;
//	}
//
//	// do the otsu global thresholding method  
//	fmax = -1.0;
//	n1 = 0;
//	for (k = 0; k <255; k++)
//	{
//		n1 += ihist[k];
//		if (!n1) { continue; }
//		n2 = n - n1;
//		if (n2 == 0) { break; }
//		csum += k *ihist[k];
//		m1 = csum / n1;
//		m2 = (sum - csum) / n2;
//		sb = n1 * n2 *(m1 - m2) * (m1 - m2);
//		/* bbg: note: can be optimized. */
//		if (sb > fmax)
//		{
//			fmax = sb;
//			thresholdValue = k;
//		}
//	}
//
//L:
//	for (i = 0; i < h; i++)
//	{
//		np = (unsigned char*)(image->imageData + image->widthStep*i);
//		for (j = 0; j < w; j++)
//		{
//			if (np[j] >= thresholdValue)
//				np[j] = 255;
//			else np[j] = 0;
//		}
//	}
//
//	//cout<<"The Threshold of this Image in Otsu is:"<<thresholdValue<<endl;  
//	return(thresholdValue);
//}
////�����и����ֱ�и����ϸ�и
//int main(int argc, char** argv) {
//	fstream outFile;
//	int up, down = 0;
//	char buffer[256];
//	char *end;
//	outFile.open("nameupdownthres.txt", ios::in);
//	outFile.getline(buffer, 256, '\n');
//	string s1(buffer);
//	up = static_cast<int>(strtol(s1.c_str(), &end, 10));
//	outFile.getline(buffer, 256, '\n');
//	string s2(buffer);
//	down = static_cast<int>(strtol(s2.c_str(), &end, 10));
//
//	char savedir[200];
//	TCHAR szFilePath[MAX_PATH + 1];
//	GetModuleFileName(NULL, szFilePath, MAX_PATH);
//	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //ɾ���ļ�����ֻ���·��
//	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
//	string dir_path(savedir);
//	dir_path += "���ָ�ͼƬ\\";
//	Directory dir;
//	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);
//
//	for (int ii = 0; ii < fileNames.size(); ii++)
//	{
//		char Savedir[200];
//		strcpy(Savedir, savedir);
//		string fileName = fileNames[ii];
//		string fileFullName = dir_path + fileName;
//		cout << "file name:" << fileName << endl;
//		cout << "file paht:" << fileFullName << endl << endl;
//
//		Mat img, img0, roi1;
//		IplImage *img_gray, *img_equaliz, *img_thread, *img_erode_dilate;
//		int min, max;
//		min = 256;
//		max = 0;
//		int thres = 0;
//		/*outFile.getline(buffer, 256, '\n');
//		string s0(buffer);
//		string sname = s0 + ".jpg";*/
//		img = imread(fileFullName);
//		cvtColor(img, img0, CV_RGB2GRAY);
//		roi1 = getroi(img0, up, down);
//		Mat binaryLineImg, grayroi1, grayroi1c;
//		cvtColor(roi1, grayroi1, CV_BGR2GRAY);
//		thres = otsu2(&IplImage(grayroi1.clone()));
//		threshold(grayroi1, binaryLineImg, thres, 255, CV_THRESH_BINARY_INV);
//		//copyһ�£��Ը���Ʒ����������ֹ�����ı�����
//		Mat binaryLineImgCopy = binaryLineImg.clone();
//		imshow("binary", binaryLineImgCopy);
//		//Ѱ����ͨ��������contours���洢���������ĵ�
//		vector<vector<Point>> contours;
//		findContours(binaryLineImgCopy,
//			contours,   //����������  
//			CV_RETR_EXTERNAL,//CV_RETR_TREE,   //��ȡ��������  
//			CV_CHAIN_APPROX_NONE);
//		//�Ƴ���������̵�����  
//		removeShortContours(contours, 2, 500);
//		Mat contoursImg = Mat(binaryLineImg.rows, binaryLineImg.cols, CV_8U);
//		drawContours(contoursImg, contours);
//		imshow("contoursImg", contoursImg);
//		//����ͨ��ת��Ϊ�����ʾ����x1��������
//		vector<int*> coordinates = CCPoints2Coordinates(contours);
//		sort(coordinates.begin(), coordinates.end(), coordinateCmp);
//		Mat ccaImg = grayroi1.clone();
//		for (int i = 0; i < coordinates.size(); i++) {
//			rectangle(ccaImg, Point(coordinates.at(i)[0], coordinates.at(i)[2]), Point(coordinates.at(i)[1], coordinates.at(i)[3]), Scalar(0, 0, 0), 0.2);
//		}
//		imshow("cca", ccaImg);
//		Mat combinedImg = grayroi1.clone();
//		vector<int*> combined = combineOverlapComponents(coordinates);
//		adjustCombinedComponent(combined);
//		for (int i = 0; i < combined.size(); i++) {
//			rectangle(combinedImg, Point(combined.at(i)[0], combined.at(i)[2]), Point(combined.at(i)[1], combined.at(i)[3]), Scalar(0, 0, 0), 0.2);
//		}
//		imshow("combine", combinedImg);
//		//�����и�
//		int LH = estimateComponentsMean(combined);
//		//Ǳ��ճ��ģʽ�����ص�ָ��ָ�򳤶�Ϊ������飬ǰ�ĸ�λ�ô洢���꣬�����λ����Ǳ��ģʽ��combined�е�index
//		vector<int*> potentialPT = detectPotentialPT(combined, LH, _thetah1, _thetah2);
//		//̽�ⵥ�ʻ�����
//		vector<vector<int*>> singleStrokes = detectSingleStroke(binaryLineImg, potentialPT, LH * 0.25, 1);
//		Mat singleStrokeImg = grayroi1.clone();
//		for (int i = 0; i < singleStrokes.size(); i++) {
//			for (int j = 0; j < singleStrokes.at(i).size(); j++) {
//				rectangle(singleStrokeImg, Point(singleStrokes.at(i).at(j)[0], singleStrokes.at(i).at(j)[2]),
//					Point(singleStrokes.at(i).at(j)[1], singleStrokes.at(i).at(j)[3]), Scalar(0, 0, 0), 0.2);
//			}
//		}
//		imshow("single_strokes", singleStrokeImg);
//		//ʹ�õ��ʻ������һ������
//		vector<int*> split = splitBySingleStroke(combined, singleStrokes);
//		Mat splitImg = grayroi1.clone();
//		for (int i = 0; i < split.size(); i++) {
//			rectangle(splitImg, Point(split.at(i)[0], split.at(i)[2]), Point(split.at(i)[1], split.at(i)[3]), Scalar(0, 0, 0), 0.2);
//		}
//		imshow("split", splitImg);
//		waitKey(0);
//		//}
//
//		/*Mat LabelImg, graylabelImg;
//		icvprCcaBySeedFill(img_thread, LabelImg);
//		LabelImg.convertTo(graylabelImg, CV_8UC1);
//		imshow("cca", graylabelImg);
//		waitKey(0);*/
//
//	}
//
//	waitKey(0);
//	return 0;
//}