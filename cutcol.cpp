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
#define MAXSIZE 5000
int summ9[10000];

int Max(int a, int b) {
	return a > b ? a : b;
}

int Min(int a, int b) {
	return a < b ? a : b;
}

int Middle(vector<int> elem, int n)
//先对数组冒泡排序，输出中间值
{
	if (n == 1) {
		return elem[0];
	}
	else
	{
		for (int i = 1; i<n; i++)
		{
			for (int j = 0; j<n - 1; j++)
			{
				if (elem[j]>elem[j + 1])
				{
					swap(elem[j], elem[j + 1]);
				}
			}
		}
		return elem[n / 2];  //如果你要找中间值，应该输出elem[n/2]
	}
	
}

Mat norm(const Mat& src) {
	// Create and return normalized image:  
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

vector<int> getWrapandCutpos(Mat src1) {
	vector<int> result;
	Mat src;
	int rindex = -1; 
	int colindex = -1;
	int minsum = 9999;
	copyMakeBorder(src1, src, src1.rows / 4, src1.rows / 4, src1.cols / 4, src1.cols / 4, BORDER_CONSTANT, Scalar(0));
	for (int r = 0; r < 1; r++) {
		Mat rotamat = getRotationMatrix2D(Point2f(src.rows / 2, src.cols / 2), r, 1);
		Mat dstresult;
		Scalar borderColor = Scalar(0);
		warpAffine(src, dstresult, rotamat, src.size(), INTER_LINEAR, BORDER_CONSTANT, borderColor);
		vector<vector<Point>> contours;
		vector<Point> contour;
		findContours(dstresult,
			contours,
			//CV_RETR_EXTERNAL,
			CV_RETR_TREE,
			CV_CHAIN_APPROX_NONE);
		for (int i = 0; i < contours.size(); i++) {
			for (int j = 0; j < contours[i].size(); j++) {
				contour.push_back(contours[i][j]);
			}
		}
		sort(contour.begin(), contour.end(), xCmp);
		int minx = contour[0].x;
		int maxx = contour[contour.size() - 1].x;
		int startx = minx + (maxx - minx) * 0.15;
		int endx = maxx - (maxx - minx) * 0.15;
		int sumcol = 9999;
		int sum = 0;
		int mini = -1;
		int startnorm = startx;
		for (int i = 0; i < contour.size(); i++) {
			if (contour[i].x >= startnorm) {
				if (contour[i].x == startx) {
					sum++;
				}
				else
				{
					if (sum < sumcol) {
						sumcol = sum;
						mini = startx;
						sum = 0;
					}
					startx++;
					i--;
				}
				if (startx > endx) {
					break;
				}
			}
		}
		if (sumcol < minsum) {
			minsum = sumcol;
			rindex = r;
			colindex = mini;
		}
		//imshow("vdbg", dstresult);
		//waitKey();
	}
	result.push_back(rindex);
	result.push_back(colindex);
	//waitKey();
	return result;
}

vector<int*> splitBySingleStroke(vector<int*> combined, vector<vector<int*>> singleStrokes) {
	if (singleStrokes.size() == 0) {
		return combined;
	}
	vector<int*> split;
	//singleStrokeIndex
	int ssi = 0;
	for (int i = 0; i < combined.size(); i++) {
		//singleStrokeIndexInCombined
		int ssiinc;
		if (ssi < singleStrokes.size()) {
			ssiinc = singleStrokes.at(ssi).at(0)[4];
		}
		else {
			ssiinc = -1;
		}
		if (i != ssiinc) {
			int* p = (int*)malloc(4 * sizeof(int));
			p[0] = combined.at(i)[0];
			p[1] = combined.at(i)[1];
			p[2] = combined.at(i)[2];
			p[3] = combined.at(i)[3];
			split.push_back(p);
		}
		else {
			int areal = combined.at(i)[0];
			int arear = combined.at(i)[1];
			int areau = combined.at(i)[2];
			int areab = combined.at(i)[3];
			int splitl, splitr;
			for (int j = 0; j < singleStrokes.at(ssi).size(); j++) {
				if (j == 0) {
					splitl = areal;
				}
				else {
					splitl = (singleStrokes.at(ssi).at(j - 1)[0] + singleStrokes.at(ssi).at(j - 1)[1]) / 2;
				}
				splitr = (singleStrokes.at(ssi).at(j)[0] + singleStrokes.at(ssi).at(j)[1]) / 2;
				int* p = (int*)malloc(4 * sizeof(int));
				p[0] = splitl;
				p[1] = splitr;
				p[2] = areau;
				p[3] = areab;
				split.push_back(p);
			}
			splitl = (singleStrokes.at(ssi).at(singleStrokes.at(ssi).size() - 1)[0] +
				singleStrokes.at(ssi).at(singleStrokes.at(ssi).size() - 1)[1]) / 2;
			splitr = arear;
			int* p = (int*)malloc(4 * sizeof(int));
			p[0] = splitl;
			p[1] = splitr;
			p[2] = areau;
			p[3] = areab;
			split.push_back(p);
			ssi++;
		}//end if
	}//end for i
	return split;
}

vector<vector<int*>> detectSingleStroke(Mat binaryLineImg, vector<int*> potentialPT, int heightThreshold, int widthThreshold) {
	vector<vector<int*>> singleStrokesForAll;
	for (int i = 0; i < potentialPT.size(); i++) {
		vector<int*> singleStrokes;
		int* p = potentialPT.at(i);
		int l = p[0], r = p[1], u = p[2], b = p[3], index = p[4];
		//1:single, 0:multi
		int* single = (int*)malloc((r - l + 1) * sizeof(int));
		bool singleStroke;
		//所有single列
		vector<int> singleArea;
		//列
		for (int j = l; j <= r; j++) {
			singleStroke = TRUE;
			//笔画开始
			bool start = FALSE;
			int startPos = 0;
			//笔画结束
			bool finish = FALSE;
			for (int k = u; k <= b; k++) {
				uchar *p = (binaryLineImg.ptr<uchar>(k) + j);
				//cout << (int)*p << ",";
			}
			//行
			for (int k = u; k <= b; k++) {
				uchar *p = (binaryLineImg.ptr<uchar>(k) + j);
				int tmp = *p;
				//白色255
				if (*p == 255 && start == FALSE) {
					start = TRUE;
					startPos = k;
					continue;
				}
				if (*p == 0 && start == TRUE && finish == FALSE) {
					finish = TRUE;
					if ((k - startPos) > heightThreshold) {
						singleStroke = FALSE;
						break;
					}
					continue;
				}
				if (*p == 255 && finish == TRUE) {
					singleStroke = FALSE;
					break;
				}
			}//end for k
			if (singleStroke)
				singleArea.push_back(j);
		}//end for j
		int start;
		if (singleArea.size() == 0) {
			continue;
		}
		//转换表示
		for (int j = 0; j < singleArea.size(); j++) {
			if (j == 0) {
				start = j;
				continue;
			}
			if (j == singleArea.size() - 1) {
				if (singleArea.at(j) == (singleArea.at(j - 1) + 1)) {
					int* oneArea = (int*)malloc(5 * sizeof(int));
					oneArea[0] = singleArea.at(start);
					oneArea[1] = singleArea.at(j);
					oneArea[4] = index;
					oneArea[2] = u;
					oneArea[3] = b;
					if (oneArea[1] - oneArea[0] >= widthThreshold)
						singleStrokes.push_back(oneArea);
				}
				else {
					int* oneArea = (int*)malloc(5 * sizeof(int));
					oneArea[0] = singleArea.at(j);
					oneArea[1] = singleArea.at(j);
					oneArea[4] = index;
					oneArea[2] = u;
					oneArea[3] = b;
					if (oneArea[1] - oneArea[0] >= widthThreshold)
						singleStrokes.push_back(oneArea);
				}
			}
			if (singleArea.at(j) != (singleArea.at(j - 1) + 1)) {
				//第一第二个数是起始点，终止点，第三第四个是上下界。第五个数是stroke所在的区域在combined中的index
				int* oneArea = (int*)malloc(5 * sizeof(int));
				oneArea[0] = singleArea.at(start);
				oneArea[1] = singleArea.at(j - 1);
				oneArea[4] = index;
				oneArea[2] = u;
				oneArea[3] = b;
				start = j;
				if (oneArea[1] - oneArea[0] >= widthThreshold)
					singleStrokes.push_back(oneArea);
			}
		}//end for j
		if (singleStrokes.size() > 0)
			singleStrokesForAll.push_back(singleStrokes);
	}//end for i
	return singleStrokesForAll;
}

vector<int*> detectPotentialPT(vector<int*> combinedCC, int LH, float thetah1, float thetah2) {
	vector<int*> potentialPT;
	for (int i = 0; i < combinedCC.size(); i++) {
		float sw = combinedCC.at(i)[1] - combinedCC.at(i)[0];
		float sh = combinedCC.at(i)[3] - combinedCC.at(i)[2];
		if (sw / sh > thetah1 || sw / LH > thetah2) {
			int* coordinate = (int*)malloc(5 * sizeof(int));
			coordinate[0] = combinedCC.at(i)[0];
			coordinate[1] = combinedCC.at(i)[1];
			coordinate[2] = combinedCC.at(i)[2];
			coordinate[3] = combinedCC.at(i)[3];
			coordinate[4] = i;
			potentialPT.push_back(coordinate);
		}
	}
	return potentialPT;
}

int estimateComponentsMean(vector<int*> combinedCC) {
	map<int, int> hist;
	map<int, int>::iterator iter;
	if (combinedCC.size() == 1) {
		return combinedCC.at(0)[3] - combinedCC.at(0)[2];
	}
	for (int i = 0; i < combinedCC.size() - 1; i++) {
		int height = ((combinedCC.at(i)[3] - combinedCC.at(i)[2]) >(combinedCC.at(i + 1)[3] - combinedCC.at(i + 1)[2])) ?
			(combinedCC.at(i)[3] - combinedCC.at(i)[2]) : (combinedCC.at(i + 1)[3] - combinedCC.at(i + 1)[2]);
		int width = combinedCC.at(i + 1)[1] - combinedCC.at(i)[0];
		if (hist.find(height) != hist.end()) {
			hist[height] = hist[height] + width;
		}
		else
		{
			hist[height] = width;
		}
	}
	iter = hist.begin();
	int widthCount = 0;
	int areaCount = 0;
	while (iter != hist.end()) {
		widthCount += iter->second;
		areaCount += (iter->first * iter->second);
		iter++;
	}
	return areaCount / widthCount;
}

void adjustCombinedComponent(vector<int*> cc) {
	for (int i = 0; i < cc.size() - 1; i++) {
		if (cc.at(i)[1] > cc.at(i + 1)[0]) {
			cc.at(i)[1] = (cc.at(i)[1] + cc.at(i + 1)[0]) / 2;
			cc.at(i + 1)[0] = (cc.at(i)[1] + cc.at(i + 1)[0]) / 2 + 1;
		}
	}
}

vector<int*> combineOverlapComponents(vector<int*> cc) {
	vector<int*> combined;
	int start = 0;
	int curxl = cc.at(0)[0], curxr = cc.at(0)[1], curyu = cc.at(0)[2], curyb = cc.at(0)[3];
	for (int i = 0; i < cc.size(); i++) {
		if (i == cc.size() - 1) {
			int* coordinate = (int*)malloc(4 * sizeof(int));
			coordinate[0] = curxl;
			coordinate[1] = curxr;
			coordinate[2] = curyu;
			coordinate[3] = curyb;
			combined.push_back(coordinate);
			break;
		}
		//i+1块是否要和i块结合
		bool combine;
		int x2l = cc.at(i + 1)[0];
		int x2r = cc.at(i + 1)[1];
		int y2u = cc.at(i + 1)[2];
		int y2b = cc.at(i + 1)[3];
		if (curxr > x2l) {
			float ovlp = curxr - x2l;
			float span = max(curxr, x2r) - curxl;
			float dist = abs((x2l + x2r) / 2 - (curxl + curxr) / 2);
			float w1 = curxr - curxl;
			float w2 = x2r - x2l;
			float nmovlp = (ovlp / w1 + ovlp / w2) / 2 - dist / span;
			//cout << i << ":" << nmovlp << "\n";
			if (nmovlp > 0.0) {
				combine = TRUE;
			}
			else {
				combine = FALSE;
			}
		}
		else {
			combine = FALSE;
		}
		if (combine) {
			curxl = min(x2l, curxl);
			curxr = max(x2r, curxr);
			curyu = min(y2u, curyu);
			curyb = max(y2b, curyb);
		}
		else {
			int* coordinate = (int*)malloc(4 * sizeof(int));
			coordinate[0] = curxl;
			coordinate[1] = curxr;
			coordinate[2] = curyu;
			coordinate[3] = curyb;
			combined.push_back(coordinate);
			curxl = x2l;
			curxr = x2r;
			curyu = y2u;
			curyb = y2b;
		}
	}
	return combined;
}

bool coordinateCmp(int *p, int *q) {
	return p[0]<q[0];
}

bool contoursizeCmp(int *p, int *q) {
	return p[1] < q[1];
}

bool xCmp(Point p1, Point p2) {
	return p1.x < p2.x;
}

bool yCmp(Point p1, Point p2) {
	return p1.y < p2.y;
}

bool widthCmp(int p, int q) {
	return p<q;
}

vector<int*> CCPoints2Coordinates(vector<vector<Point>> cc) {
	vector<int*> coordinates;
	for (int i = 0; i < cc.size(); i++) {
		int x1, x2 = 0, y1, y2 = 0;
		x1 = MAXSIZE;
		y1 = MAXSIZE;
		int numPoint = cc[i].size();
		for (int j = 0; j < numPoint; j++) {
			if (cc[i][j].x < x1) {
				x1 = cc[i][j].x;
			}
			if (cc[i][j].x > x2) {
				x2 = cc[i][j].x;
			}
			if (cc[i][j].y < y1) {
				y1 = cc[i][j].y;
			}
			if (cc[i][j].y > y2) {
				y2 = cc[i][j].y;
			}
		}
		int* coordinate = (int*)malloc(4 * sizeof(int));
		coordinate[0] = x1;
		coordinate[1] = x2;
		coordinate[2] = y1;
		coordinate[3] = y2;
		coordinates.push_back(coordinate);
	}
	return coordinates;
}

vector<int*> CCPoints2Coordinates1(vector<vector<Point>> cc) {
	vector<int*> coordinates;
	for (int i = 0; i < cc.size(); i++) {
		int x1, x2 = 0, y1, y2 = 0;
		x1 = MAXSIZE;
		y1 = MAXSIZE;
		x2 = -99999;
		y2 = -99999;
		int numPoint = cc[i].size();
		for (int j = 0; j < numPoint; j++) {
			if (cc[i][j].x < x1) {
				x1 = cc[i][j].x;
			}
			if (cc[i][j].x > x2) {
				x2 = cc[i][j].x;
			}
			if (cc[i][j].y < y1) {
				y1 = cc[i][j].y;
			}
			if (cc[i][j].y > y2) {
				y2 = cc[i][j].y;
			}
		}
		int* coordinate = (int*)malloc(6 * sizeof(int));
		coordinate[0] = x1;
		coordinate[1] = x2;
		coordinate[2] = y1;
		coordinate[3] = y2;
		coordinate[4] = i;
		coordinate[5] = -1;
		coordinates.push_back(coordinate);
	}
	return coordinates;
}

vector<int*> combinecoord(vector<int*> coordinates) {
	vector<int*> newcoordinates;
	newcoordinates.push_back(coordinates[0]);
	for (int i = 1; i < coordinates.size(); i++) {
		/*float rotio = ((coordinates[i][1] * 1.0) - (coordinates[i][0] * 1.0)) / 
			((coordinates[i][3] * 1.0) - (coordinates[i][2] * 1.0));*/
		int down = coordinates[i][3];
		int up = coordinates[i - 1][2];
		int premid = (coordinates[i - 1][2] + coordinates[i - 1][3]) / 2;//if (rotio > 1.9) {
		if ((down < premid)) {
			int* coordinate = (int*)malloc(6 * sizeof(int));
			int prex1 = newcoordinates[newcoordinates.size() - 1][0];
			int prex2 = newcoordinates[newcoordinates.size() - 1][1];
			int prey1 = newcoordinates[newcoordinates.size() - 1][2];
			int prey2 = newcoordinates[newcoordinates.size() - 1][3];
			int c = newcoordinates[newcoordinates.size() - 1][4];
			coordinate[0] = prex1;
			coordinate[1] = Max(coordinates[i][1], prex2);
			coordinate[2] = Min(coordinates[i][2], prey1);
			coordinate[3] = Max(coordinates[i][3], prey2);
			coordinate[4] = c;
			coordinate[5] = coordinates[i][4];
			newcoordinates.pop_back();
			newcoordinates.push_back(coordinate);
		}
		else {
			newcoordinates.push_back(coordinates[i]);
		}
	}
	return newcoordinates;
}

vector<int*> combinecoord1(vector<int*> coordinates) {
	vector<int*> newcoordinates;
	newcoordinates.push_back(coordinates[0]);
	for (int i = 1; i < coordinates.size(); i++) {
		if ((coordinates[i][0] < newcoordinates[newcoordinates.size() - 1][1])) {
			int* coordinate = (int*)malloc(6 * sizeof(int));
			int prex1 = newcoordinates[newcoordinates.size() - 1][0];
			int prex2 = newcoordinates[newcoordinates.size() - 1][1];
			int prey1 = newcoordinates[newcoordinates.size() - 1][2];
			int prey2 = newcoordinates[newcoordinates.size() - 1][3];
			int c = newcoordinates[newcoordinates.size() - 1][4];
			coordinate[0] = prex1;
			coordinate[1] = Max(coordinates[i][1], prex2);
			coordinate[2] = Min(coordinates[i][2], prey1);
			coordinate[3] = Max(coordinates[i][3], prey2);
			coordinate[4] = c;
			coordinate[5] = coordinates[i][4];
			newcoordinates.pop_back();
			newcoordinates.push_back(coordinate);
		}
		else {
			newcoordinates.push_back(coordinates[i]);
		}
	}
	return newcoordinates;
}

void drawContours(Mat img, vector<vector<Point>>contours) {
	uchar color = 0;
	for (int i = 0; i < contours.size(); i++) {
		color = color > 150 ? 0 : color + 30;
		//cout << (int)color << "!";
		for (int j = 0; j < contours[i].size(); j++) {
			Point point = contours[i][j];
			int px = point.x;
			int py = point.y;
			uchar *p = (img.ptr<uchar>(py) + px);
			*p = color;
		}
	}
}

void removeShortContours(vector<vector<Point>>& contours, int cmin, int cmax) {
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else {
			vector<Point> con = *itc;
			int conSize = con.size();
			int count = 0;
			int norm = con[0].y;
			for (int i = 0; i < conSize; i++) {
				if (norm == con[i].y)
					count++;
			}
			if (count >= (conSize * 0.7))
				itc = contours.erase(itc);
			else
			{
				++itc;
			}
		}
	}

}

vector<Point> getMaxContours(vector<vector<Point>> contours) {
	if (contours.size() != 1) {
		int maxlen = -9999;
		int index = -1;
		for (int i = 0; i < contours.size(); i++) {
			int len = contours[i].size();
			if (len > maxlen) {
				maxlen = len;
				index = i;
			}
		}
		return contours[index];
	}
	else
	{
		return contours[0];
	}
}

vector<vector<int>> contoursRows(vector<Point> contour, int y1, int y2) {
	vector<vector<int>> result;
	for (int j = y1; j < y2 + 1; j++) {
		vector<int> rowindex;
		vector<int> finarowindex;
		for (int i = 0; i < contour.size(); i++) {
			if (contour[i].y == j) {
				rowindex.push_back(contour[i].x);
			}
		}
		sort(rowindex.begin(), rowindex.end(), widthCmp);
		if (rowindex.size() == 1) {
			finarowindex.push_back(rowindex[0]);
		}
		else if (rowindex.size() == 0)
		{

		}
		else
		{
			finarowindex.push_back(rowindex[0]);
			finarowindex.push_back(rowindex[rowindex.size() - 1]);
		}
		result.push_back(finarowindex);
	}
	return result;
}

int otsu2(IplImage *image)
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

void deleteLines(Mat binLine) {
	int lineLength = 30;
	for (int i = 0; i < binLine.rows; i++) {
		vector<int> blackdot;
		for (int j = 0; j < binLine.cols - 1; j++) {
			if ((binLine.at<uchar>(i, j) == 255) && (binLine.at<uchar>(i, j + 1) == 255)) {
				blackdot.push_back(j);
			}
			else
			{
				int count = blackdot.size();
				if (count > lineLength) {
					for (int j1 = 0; j1 < count; j1++) {
						binLine.at<uchar>(i, blackdot[j1]) = 0;
					}
				}
				else
				{
					vector<int>().swap(blackdot);
				}
			}
		}
	}
}

void RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;       //记录除去的个数  
							   //记录每个像素点检验状态的标签，0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查  
	Mat Pointlabel = Mat::zeros(Src.size(), CV_8UC1);

	if (CheckMode == 1)
	{
		//cout << "Mode: 去除小区域. ";
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] < 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}
	else
	{
		//cout << "Mode: 去除孔洞. ";
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] > 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}

	vector<Point2i> NeihborPos;  //记录邻域点位置  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeihborMode == 1)
	{
		//cout << "Neighbor mode: 8邻域." << endl;
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	else cout << "Neighbor mode: 4邻域." << endl;
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//开始检测  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 0)
			{
				//********开始该点处的检查**********  
				vector<Point2i> GrowBuffer;                                      //堆栈，用于存储生长点  
				GrowBuffer.push_back(Point2i(j, i));
				Pointlabel.at<uchar>(i, j) = 1;
				int CheckResult = 0;                                               //用于判断结果（是否超出大小），0为未超出，1为超出  

				for (int z = 0; z<GrowBuffer.size(); z++)
				{

					for (int q = 0; q<NeihborCount; q++)                                      //检查四个邻域点  
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //防止越界  
						{
							if (Pointlabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY));  //邻域点加入buffer  
								Pointlabel.at<uchar>(CurrY, CurrX) = 1;           //更新邻域点的检查标签，避免重复检查  
							}
						}
					}

				}
				if (GrowBuffer.size()>AreaLimit) CheckResult = 2;                 //判断结果（是否超出限定的大小），1为未超出，2为超出  
				else { CheckResult = 1;   RemoveCount++; }
				for (int z = 0; z<GrowBuffer.size(); z++)                         //更新Label记录  
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
				}
				//********结束该点处的检查**********  


			}
		}
	}

	CheckMode = 255 * (1 - CheckMode);
	//开始反转面积过小的区域  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iData = Src.ptr<uchar>(i);
		uchar* iDstData = Dst.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 2)
			{
				iDstData[j] = CheckMode;
			}
			else if (iLabel[j] == 3)
			{
				iDstData[j] = iData[j];
			}
		}
	}
}

vector<Vec4i> drawLinesforPic(Mat gray, int len) {
	Mat CannyImg;
	Canny(gray, CannyImg, 140, 250, 3);
	vector<Vec4i> Lines;
	HoughLinesP(CannyImg, Lines, 1, CV_PI / 180, 170, len, 30);
	vector<Vec4i>::const_iterator itc = Lines.begin();
	while (itc != Lines.end())
	{
		if (abs((*itc)[1] - (*itc)[3]) > 20)
			itc = Lines.erase(itc);
		else
		{
			++itc;
		}
	}
	return Lines;
}

vector<Vec4i> drawLinesforPic1(Mat gray, int len) {
	Mat CannyImg;
	Canny(gray, CannyImg, 140, 250, 3);
	vector<Vec4i> Lines;
	HoughLinesP(CannyImg, Lines, 1, CV_PI / 180, 80, len, 10);
	vector<Vec4i>::const_iterator itc = Lines.begin();
	while (itc != Lines.end())
	{
		if (abs((*itc)[0] - (*itc)[2]) > 0)
			itc = Lines.erase(itc);
		else
		{
			++itc;
		}
	}
	return Lines;
}

Mat splitTored(Mat src) {
	vector<Mat> channels;
	split(src, channels);
	Mat red = channels[2];
	Mat blue = channels[0];
	Mat green = channels[1];
	return red;
}

void setPathOfexe(char *savedir) {
	//char savedir[200];
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	int iLen = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, savedir, iLen, NULL, NULL);
	//return savedir;
}

vector<int> getcutPoint(vector<int*> split, Mat fgImg) {
	vector<int> fg;
	int prefgpoint = 0;
	for (int i = 0; i < (split.size() - 1); i++) {
		if (i == 0)
			prefgpoint = split.at(i)[0];
		int fgpoint = (split.at(i)[1] + split.at(i + 1)[0]) / 2;
		prefgpoint = fgpoint;
		fg.push_back(fgpoint);
	}
	fg.insert(fg.begin(), 0);
	fg.insert(fg.end(), fgImg.cols);
	return fg;
}

//去掉上方空白
int getCutfromup(Mat src) {
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	int y1 = 0;
	int y2 = 0;
	bool flag1 = false;
	bool flag2 = false;
	for (int i = 0; i < height; i++) {
		int sum = 0;
		for (int j = 0; j < width; j++) {
			sum = sum + (int)ptr[i * img->widthStep + j];
		}
		if ((!flag1) && (!flag2)) {
			if (sum == 0) {
				y1 = i;
				flag1 = true;
			}
		}
		else
		{
			if (sum > 0) {
				y2 = i;
				flag2 = true;
				break;
			}
		}
	}
	if (flag1 && flag2) {
		int cha = y2 - y1;
		float charoia = cha / (float)src.rows;
		if (charoia > 0.18)
			return y2;
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
//去掉下方空白
int getCutfromdown(Mat src) {
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	int y1 = height - 1;
	int y2 = height - 1;
	bool flag1 = false;
	//bool flag2 = false;
	for (int i = height - 1; i >= 0; i--) {
		int sum = 0;
		for (int j = 0; j < width; j++) {
			sum = sum + (int)ptr[i * img->widthStep + j];
		}
		if (!flag1) {
			if (sum > 0) {
				y1 = i;//首次出现字
				flag1 = true;
				break;
			}
		}

	}
	if (flag1) {
		int cha = y2 - y1;
		float charoia = cha / (float)src.rows;
		if (charoia > 0.18)
			return y1;
		else
		{
			return y2;
		}
	}
	else
	{
		return 0;
	}
}
Mat CutfromLeftandRight(Mat src) {
	Mat result;
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	bool flag1 = false;
	int x1 = 0;
	int x2 = width;
	for (int i = 0; i < width / 2; i++) {
		int sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];
		}
		if (sum > 0)
			flag1 = true;
		if (flag1)
			if (sum == 0) {
				x1 = i;//缝的左边界
				break;
			}
	}
	flag1 = false;
	for (int i = width - 1; i > width / 2; i--) {
		int sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];
		}
		if (sum > 0)
			flag1 = true;
		if (flag1)
			if (sum == 0) {
				x2 = i;//缝的右边界
				break;
			}
	}
	result = src(Range(0, src.rows), Range(x1, x2));
	return result;
}


void savePicture(vector<int> fg, string filename, char* Savedir, Mat fgImg) {
	Mat *fgroi = new Mat[fg.size() - 1];
	char save_file[200];
	string str = "分割\\" + filename.substr(0, filename.length() - 4) + "\\";
	char *p = (char*)str.data();
	strcat(Savedir, p);
	FILE *fp = NULL;
	fp = fopen(Savedir, "w");
	if (!fp)
		mkdir(Savedir);
	for (int i = 0; i < (fg.size() - 1); i++) {
		fgroi[i] = fgImg(Range(0, fgImg.rows), Range(fg[i], fg[i + 1]));
		sprintf(save_file, "%s%d.jpg", Savedir, i);
		imwrite(save_file, fgroi[i]);
	}
}

Mat inhance(Mat src, float gamma) {
	Mat X, I;
	src.convertTo(X, CV_32FC1);
	pow(X, gamma, I);
	return norm(I);
}

void quickSort(int s[], int l, int r)
{
	if (l< r)
	{
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while (i < j && s[j] >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && s[i]< x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // 递归调用  
		quickSort(s, i + 1, r);
	}
}

void thinimage(Mat &srcimage)//单通道、二值化后的图像
{
	vector<Point> deletelist1;
	int Zhangmude[9];
	int nl = srcimage.rows;
	int nc = srcimage.cols;
	while (true)
	{
		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Zhangmude[0] = 1;
					if (data_last[i] == 255) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if (data_last[i + 1] == 255) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if (data[i + 1] == 255) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if (data_next[i + 1] == 255) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if (data_next[i] == 255) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if (data_next[i - 1] == 255) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if (data[i - 1] == 255) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if (data_last[i - 1] == 255) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
						if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
						if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
						if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
						if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
						if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
						if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
						if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();

		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Zhangmude[0] = 1;
					if (data_last[i] == 255) Zhangmude[1] = 1;
					else  Zhangmude[1] = 0;
					if (data_last[i + 1] == 255) Zhangmude[2] = 1;
					else  Zhangmude[2] = 0;
					if (data[i + 1] == 255) Zhangmude[3] = 1;
					else  Zhangmude[3] = 0;
					if (data_next[i + 1] == 255) Zhangmude[4] = 1;
					else  Zhangmude[4] = 0;
					if (data_next[i] == 255) Zhangmude[5] = 1;
					else  Zhangmude[5] = 0;
					if (data_next[i - 1] == 255) Zhangmude[6] = 1;
					else  Zhangmude[6] = 0;
					if (data[i - 1] == 255) Zhangmude[7] = 1;
					else  Zhangmude[7] = 0;
					if (data_last[i - 1] == 255) Zhangmude[8] = 1;
					else  Zhangmude[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Zhangmude[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
						if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
						if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
						if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
						if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
						if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
						if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
						if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();
	}
}

void saverecPic(vector<int*> split, string filename, char* Savedir, Mat fgImg) {
	Mat *fgroi = new Mat[split.size()];
	char save_file[200];
	string str = "分割\\" + filename.substr(0, filename.length() - 4) + "\\";
	char *p = (char*)str.data();
	strcat(Savedir, p);
	FILE *fp = NULL;
	fp = fopen(Savedir, "w");
	if (!fp)
		mkdir(Savedir);
	if (fgImg.rows == 81) {
		Mat result;
		int thres = otsu2(&IplImage(fgImg.clone()));
		threshold(fgImg, result, thres, 255, CV_THRESH_BINARY_INV);
		RemoveSmallRegion(result, result, 8, 1, 1);
		bitwise_not(result, result);
		sprintf(save_file, "%s%d.jpg", Savedir, 0);
		imwrite(save_file, result);
	}
	else
	{
		for (int i = 0; i < (split.size()); i++) {
			Mat result, inhanceImg;
			int thres;
			fgroi[i] = fgImg(Range(split.at(i)[2], split.at(i)[3]), Range(split.at(i)[0], split.at(i)[1]));
			if (((fgroi[i].rows * fgroi[i].cols) < 400) &&
				(((float)fgroi[i].rows / fgroi[i].cols) < 1.5) && (((float)fgroi[i].cols / fgroi[i].rows) < 1.5)) {
				inhanceImg = inhance(fgroi[i], 8.0);
			}
			else
			{
				inhanceImg = fgroi[i];
			}
			thres = otsu2(&IplImage(inhanceImg.clone()));
			threshold(inhanceImg, result, thres, 255, CV_THRESH_BINARY_INV);
			sprintf(save_file, "%s%d.jpg", Savedir, i);
			if ((result.rows > 7) && (result.cols > 7)) {
				int a = getCutfromup(result);
				Mat fresult1 = result(Range(a, result.rows), Range(0, result.cols));
				Mat fresult2 = CutfromLeftandRight(fresult1);
				//thinimage(fresult2);
				//imshow("xihua", fresult2);
				//waitKey(0);
				bitwise_not(fresult2, fresult2);
				imwrite(save_file, fresult2);
			}
		}
	}
}

Mat matrixWiseMulti(Mat &m1, Mat &m2) {
	Mat dst = m1.mul(m2);//注意是对应矩阵位置的元素相乘
	return dst;
}

//图像局部对比度增强算法
//float MaxCG:对高频成分的最大增益值
//int n：局部半径
//int C；对高频的直接增益系数
//Mat src：原图像
Mat ACE(Mat &src, int C, int n, float MaxCG) {
	int rows = src.rows;
	int cols = src.cols;

	Mat meanLocal;
	Mat varLocal;
	Mat meanGlobal;
	Mat varGlobal;

	blur(src.clone(), meanLocal, Size(n, n));//meanMask为图像局部均值 
	imshow("低通滤波", meanLocal);
	Mat highFreq = src - meanLocal;//高频成分：x(i,j)-Mx(i,j)
	imshow("高频成分", highFreq);

	varLocal = matrixWiseMulti(highFreq, highFreq);
	blur(varLocal, varLocal, Size(n, n));    //varMask为此时为图像局部方差   
											 //换算成局部标准差（开根号）
	varLocal.convertTo(varLocal, CV_32F);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			varLocal.at<float>(i, j) = (float)sqrt(varLocal.at<float>(i, j));
		}
	}
	meanStdDev(src, meanGlobal, varGlobal); //meanGlobal为全局均值 varGlobal为全局标准差，实际均是一个数
	Mat gainArr = 0.5 * meanGlobal / varLocal;//增益系数矩阵：G(i,j)，可以直接令其为系数C（一般总是大于1）
											  /*
											  for (int i = 0; i < rows; i++){
											  for (int j = 0; j < cols; j++)
											  cout<<gainArr.at<float>(i, j)<<" " ;
											  cout << endl;
											  if (i == 1)
											  break;
											  }*/

											  //对增益矩阵进行截止
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (gainArr.at<float>(i, j) > MaxCG) {
				gainArr.at<float>(i, j) = MaxCG;
			}
		}
	}
	gainArr.convertTo(gainArr, CV_8U);
	gainArr = matrixWiseMulti(gainArr, highFreq);
	Mat dst1 = meanLocal + gainArr;
	imshow("Lee改进的D方法", dst1);
	Mat dst2 = meanLocal + C*highFreq;//直接利用系数C进行高频成分放大
	imshow("直接系数C方法", dst2);
	return dst2;
}

Mat myACE(Mat &src, int n, float MaxCG) {
	int rows = src.rows;
	int cols = src.cols;
	Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0));

	if (src.type() == CV_8UC1)
		int aa = src.type();
	Mat meanLocal;
	Mat varLocal;
	Mat meanGlobal;
	Mat varGlobal;

	blur(src.clone(), meanLocal, Size(n, n));//meanMask为图像局部均值 
	Mat highFreq = src - meanLocal;//高频成分：x(i,j)-Mx(i,j)

	varLocal = matrixWiseMulti(highFreq, highFreq);
	blur(varLocal, varLocal, Size(n, n));    //varMask为此时为图像局部方差   
											 //换算成局部标准差（开根号）
	varLocal.convertTo(varLocal, CV_32F);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			varLocal.at<float>(i, j) = (float)sqrt(varLocal.at<float>(i, j));
		}
	}
	meanStdDev(src, meanGlobal, varGlobal); //meanGlobal为全局均值 varGlobal为全局标准差，实际均是一个数
	Mat gainArr = 0.5 * meanGlobal / varLocal;//增益系数矩阵：G(i,j)，可以直接令其为系数C（一般总是大于1）
											  //对增益矩阵进行截止
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (gainArr.at<float>(i, j) > MaxCG) {
				gainArr.at<float>(i, j) = MaxCG;
			}
		}
	}
	gainArr.convertTo(gainArr, CV_8U);
	gainArr = matrixWiseMulti(gainArr, highFreq);
	dst = meanLocal + gainArr;
	//imshow("Lee改进的D方法", dst);
	return dst;
}

int FindByTwo(int *nums, int number)
{
	int mid;
	int left = 0;
	int right = sizeof(nums) / sizeof(nums[0]) - 1;
	while (left != right)
	{
		mid = (left + right) / 2;
		if (number > nums[mid])
		{
			if (number < nums[mid + 1])
				return nums[mid];
			else
			{
				left = mid + 1;
			}
		}
		if (number < nums[mid])
		{
			if (number > nums[mid - 1])
				return nums[mid - 1];
			else
			{
				right = mid - 1;
			}
		}
	}
}

void getAllFiles(string path, vector<string>& files)
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

vector<int> getCutfromleftandright(Mat src) {
	vector<int> two;
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	bool flag = false;
	for (int i = 0; i < width; i++) {//从左往右找左间隙
		int sum = 0;//找缝隙，sum=0的列
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];
			if (sum > 0) {
				flag = true;
				break;
			}
		}
		if (sum == 0)
			if (flag) {
				two.push_back(i);//记录间隙所在列，不在继续往下走
				break;
			}
	}
	flag = false;
	for (int i = width - 1; i > 0; i--) {//从右往左找右间隙
		int sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];
			if (sum > 0) {
				flag = true;
				break;
			}
		}
		if (sum == 0)
			if (flag) {
				two.push_back(i);////记录间隙所在列，不在继续往下走
				break;
			}
	}
	if (two.size() == 1) {
		two.clear();
	}
	return two;
}

vector<int> getCutfromupanddown(Mat src) {
	vector<int> two;
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	bool flag = false;
	for (int i = 0; i < height / 2; i++) {//从上往下找上间隙
		int sum = 0;//找缝隙，sum=0的列
		for (int j = 0; j < width; j++) {
			sum = sum + (int)ptr[i * img->widthStep + j];
			if (sum > 0) {
				flag = true;
				break;
			}
		}
		if (sum == 0)
			if (flag) {
				two.push_back(i);//记录间隙所在行，不在继续往下走
				break;
			}
	}
	flag = false;
	for (int i = height - 1; i > height / 2; i--) {//从下往上找下间隙
		int sum = 0;
		for (int j = 0; j < width; j++) {
			sum = sum + (int)ptr[i * img->widthStep + j];
			if (sum > 0) {
				flag = true;
				break;
			}
		}
		if (sum == 0)
			if (flag) {
				two.push_back(i);////记录间隙所在行，不在继续往下走
				break;
			}
	}
	return two;
}

vector<int> getCenterCutLAndR(Mat src) {
	vector<int> two;
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	//bool flag = false;
	for (int i = 0; i < width; i++) {//从左往右找首次出现字的地方
		int sum = 0;//找缝隙，sum=0的列
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];
		}
		if (sum > 0) {
			two.push_back(i);
			//flag = true;
			break;
		}
	}
	//flag = false;
	for (int i = width - 1; i > 0; i--) {//从右往左找右间隙
		int sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j * img->widthStep + i];

		}
		if (sum > 0) {
			two.push_back(i);
			break;
		}
	}
	return two;
}


bool firstwhite(Mat src) {
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	int sum = 0;
	for (int i = 0; i < height; i++) {
		sum = sum + (int)ptr[i * img->widthStep + 0];
	}
	if (sum > 0)
		return true;
	else
	{
		return false;
	}
}

vector<int> getcutpos(IplImage *img, int shrink) {
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
		for (int j = 0 + shrink; j < height - shrink; j++) {
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
		for (int j = 0 + shrink; j < height - shrink; j++) {
			sum = sum + (int)ptr[j*img->widthStep + i];
			if (sum > 0) {
				x2 = i + 1;
				t = 1;
			}
			if (t == 1)
				break;
		}
		if (t == 1)
			break;
	}
	t = 0;
	for (int i = 0 + shrink; i < height; i++) {
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
	for (int i = height - 1 - shrink; i > 0; i--) {
		int sum = 0;
		for (int j = 0 + shrink; j < width - shrink; j++) {
			sum = sum + (int)ptr[i*img->widthStep + j];
			if (sum > 0) {
				y2 = i + 1;
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

int traverseCutPoint(Mat src) {
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	int index = -1;
	int value = 9999999;
	for (int i = 0.2 * width; i < width * 0.8; i++) {
		int sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + (int)ptr[j*img->widthStep + i];
		}
		if (sum < value) {
			index = i;
			value = sum;
		}
	}
	return index;
}

bool whitepoint(Mat src) {
	IplImage *img = &IplImage(src);
	uchar *ptr = (uchar *)img->imageData;
	int width = img->width;
	int height = img->height;
	/*int total = width * height;
	int count = 0;
	for (int i = 0; i < width; i++)
	for (int j = 0; j < height; j++) {
	if ((int)ptr[j*img->widthStep + i] == 255)
	count++;
	}
	float roito = count * 1.0 / total;
	if (roito > 0.5) {
	return true;
	}
	else
	{
	return false;
	}*/
	if ((width <= 25) && (height <= 25))
		return true;
	else
		return false;
}

vector<int*> combinecombined(vector<int*> com, Mat src) {
	Mat test1 = src.clone();
	Mat test2 = src.clone();
	Mat test3 = src.clone();
	Mat test_1 = src.clone();
	Mat src1 = src.clone();
	Mat src2 = src.clone();
	Mat src3 = src.clone();
	int maxf = 15;
	float maxthinrotio = 0.18;
	float maxrotio = 1.2;
	float minrotio = 0.5;
	float Hhrotio = 1.68;//0.5;//大高/小高
	float cutWandTotalW = 0.3;//缝宽占总宽
	float LRwithTotalW = 0.2;//左、右半部分比总宽

	vector<int*> newcom;
	vector<int*> newcom1;//存放合并掉细条后的轮廓
	vector<int*> newcom2;//对大于1.2的判断是否切割，存放最后结果
	vector<int*> newcom3;
	vector<int*> newcom4;
	int count1 = 0;
	for (int i = 0; i < com.size() - 1; i = i + count1 + 1) {
		count1 = 0;
		int *a = new int[4];
		a[0] = com.at(i)[0];
		a[1] = com.at(i)[1];
		a[2] = com.at(i)[2];
		a[3] = com.at(i)[3];
	again:
		if (a[1] >= com.at(i + 1 + count1)[0]) {
			int x1 = a[0];
			int x2 = com.at(i + 1 + count1)[1];
			int y1;
			int y2;
			y1 = Min(a[2], com.at(i + 1 + count1)[2]);
			y2 = Max(a[3], com.at(i + 1 + count1)[3]);
			a[0] = x1;
			a[1] = x2;
			a[2] = y1;
			a[3] = y2;
			count1++;
			if ((i + count1 + 1) == com.size()) {
				newcom.push_back(a);
				break;
			}
			goto again;
		}
		else
		{
			newcom.push_back(a);
			if ((i + count1 + 1) == (com.size() - 1))
				newcom.push_back(com.at(com.size() - 1));
		}
	}
	/*for (int i = 0; i < newcom.size(); i++) {
	rectangle(test1, Point(newcom.at(i)[0], newcom.at(i)[2]), Point(newcom.at(i)[1], newcom.at(i)[3]), Scalar(255, 255, 255), 0.2);
	}
	imwrite("C:\\Users\\Administrator\\Desktop\\test112.jpg", test1);
	waitKey(0);*/


	int count = 0;
	for (int i = 0; i < newcom.size(); i = i + count + 1) {
		count = 0;
		float width = newcom.at(i)[1] - newcom.at(i)[0];
		float height = newcom.at(i)[3] - newcom.at(i)[2];
		float rotio = width / height;
		int *r = new int[4];
		r[0] = newcom[i][0];
		r[1] = newcom[i][1];
		r[2] = newcom[i][2];
		r[3] = newcom[i][3];

		Mat point = src(Range(r[2], r[3]), Range(r[0], r[1]));
		bool ispoint = whitepoint(point);
		if ((rotio <= minrotio) || ispoint) {//小于0.5
			if (i == 0) {//向右合并
			again1:
				int disf = newcom[i + count + 1][0] - newcom[i + count][1];
				if ((width / height < minrotio) && (disf < maxf)) {//向右合并
					int x1 = r[0];
					int x2 = newcom.at(i + 1 + count)[1];
					int y1;
					int y2;
					y1 = Min(r[2], newcom.at(i + 1 + count)[2]);
					y2 = Max(r[3], newcom.at(i + 1 + count)[3]);
					r[0] = x1;
					r[1] = x2;
					r[2] = y1;
					r[3] = y2;
					++count;//合并次数
					width = r[1] - r[0];
					height = r[3] - r[2];
					if ((i + count + 1) == newcom.size()) {
						newcom1.push_back(r);
						break;
					}
					goto again1;
				}
				else
				{
					newcom1.push_back(r);
					if ((i + count + 1) == (newcom.size() - 1))
						newcom1.push_back(newcom.at(newcom.size() - 1));
					continue;
				}
			}//连续向右合并结束，开始下一个轮廓

			if (i == (newcom.size() - 1)) {//最后一个只能向左合并

										   //again2:
				int disf = newcom[i][0] - newcom1[newcom1.size() - 1][1];
				if ((width / height < minrotio) && (disf < maxf)) {//向左合并

					int x1 = newcom1[newcom1.size() - 1][0];
					int x2 = r[1];
					int y1;
					int y2;
					y1 = Min(newcom1[newcom1.size() - 1][2], r[2]);
					y2 = Max(newcom1[newcom1.size() - 1][3], r[3]);
					r[0] = x1;
					r[1] = x2;
					r[2] = y1;
					r[3] = y2;
					//++count;//合并次数
					width = r[1] - r[0];
					height = r[3] - r[2];
					newcom1.pop_back();
					newcom1.push_back(r);
					//goto again2;
				}
				else
				{
					newcom1.push_back(r);

					break;
				}
			}
			else
			{//中间情况可以向两边合并
			 //左合并
			again3:
				if (width / height < minrotio || ispoint) {
					//float w1 = newcom[i][1] - newcom1[newcom1.size() - 1][0];
					//float h1 = Max(newcom1[newcom1.size() - 1][3], newcom[i][3]) - Min(newcom1[newcom1.size() - 1][2], newcom[i][2]);
					//float rotio1 = w1 / h1;
					////右合并
					//float w2 = newcom[i + 1][1] - newcom[i][0];
					//float h2 = Max(newcom[i + 1][3], newcom[i][3]) - Min(newcom[i + 1][2], newcom[i][2]);
					//float rotio2 = w2 / h2;
					//向左合并还是右，取决于哪一个中间缝隙窄
					//向左合并，间隙宽
					ispoint = false;
					float cutLW, cutRW;//向左、右合并，间隙宽
					if (newcom1.size() == 0)
					{
						cutLW = 99999;
					}
					else {
						cutLW = r[0] - newcom1[newcom1.size() - 1][1];
					}
					if (i + count == newcom.size() - 1)
					{
						cutRW = 99999;
					}
					else {
						cutRW = newcom[i + count + 1][0] - r[1];
					}

					//int *r = new int[4];
					if ((cutLW >= cutRW) && (cutRW < maxf))
					{//向右合并

						r[0] = r[0];
						r[1] = newcom[i + count + 1][1];
						r[2] = Min(newcom[i + count + 1][2], r[2]);
						r[3] = Max(newcom[i + count + 1][3], r[3]);

						++count;

					}
					if ((cutLW < cutRW) && (cutLW < maxf))
					{//向左合并
						int tempx = newcom1[newcom1.size() - 1][0];//与当前newcom1中最后一个合并
						int tempy1 = newcom1[newcom1.size() - 1][2];
						int tempy2 = newcom1[newcom1.size() - 1][3];


						r[0] = tempx;
						r[1] = r[1];
						r[2] = Min(tempy1, r[2]);
						r[3] = Max(tempy2, r[3]);


						newcom1.pop_back();//去除当前最后一个
					}
					if ((cutLW >= maxf) && (cutRW >= maxf)) {
						newcom1.push_back(r);
						continue;
					}
					width = r[1] - r[0];
					height = r[3] - r[2];
					goto again3;
				}
				else
				{
					newcom1.push_back(r);
					continue;
				}

			}
		}
		else
		{//宽/高>=0.4,再判断是否是点，如果是点再结合，否则不结合

			newcom1.push_back(newcom[i]);
			count = 0;
			//确定白点像素总数/总像素数的比>0.5，是点结合，否则不用结合
			/*

			if (true)
			{

			}
			else {

			}*/

		}

	}
	vector<int> widths;//收集所有小于1.2的宽度，排序求中值
	for (int i = 0; i < newcom1.size(); i++)
	{
		float w = newcom1.at(i)[1] - newcom1.at(i)[0];
		float h = newcom1.at(i)[3] - newcom1.at(i)[2];
		float rotio = w / h;
		if (rotio < maxrotio) {
			widths.push_back(w);
		}
	}
	sort(widths.begin(), widths.end(), widthCmp);
	float meanWidth = widths.at(widths.size() / 2);

	/*for (int i = 0; i < newcom1.size(); i++) {
	rectangle(test3, Point(newcom1.at(i)[0], newcom1.at(i)[2]), Point(newcom1.at(i)[1], newcom1.at(i)[3]), Scalar(255, 255, 255), 0.2);
	}
	imshow("test3.jpg", test3);
	waitKey(0);*/
	int count3 = 0;
	for (int i = 0; i < newcom1.size(); i = i + count3 + 1) {
		count3 = 0;
		float w = newcom1.at(i)[1] - newcom1.at(i)[0];
		float h = newcom1.at(i)[3] - newcom1.at(i)[2];
		float rotio = w / h;
		float max = w - meanWidth ? w : meanWidth;
		float min = w - meanWidth ? meanWidth : w;
		float sd = max / min;
		if (rotio >= maxrotio) {//大于1.2 的判断是否切割
			Mat adh = src(Range(newcom1.at(i)[2], newcom1.at(i)[3]), Range(newcom1.at(i)[0], newcom1.at(i)[1]));

			vector<int> lr = getCutfromleftandright(adh);//返回两个间隙所在列
			if (lr.size() == 0) {
				//没有间隙，找投影和最小的列
				//拆分为左右
				//int *left = new int[4];
				//int *right = new int[4];
				//left[0] = newcom1.at(i)[0];
				//left[1] = traverseCutPoint(adh) + newcom1.at(i)[0];//traverseCutPoint返回最小sum所在列
				//left[2] = newcom1.at(i)[2];
				//left[3] = newcom1.at(i)[3];
				//right[0] = left[1];
				//right[1] = newcom1.at(i)[1];
				//right[2] = newcom1.at(i)[2];
				//right[3] = newcom1.at(i)[3];
				//
				//Mat adhleft = src1(Range(left[2], left[3]), Range(left[0], left[1]));
				//Mat adhright = src2(Range(right[2], right[3]), Range(right[0], right[1]));
				//
				////计算左右两部分去掉上方空白后的高度
				////getCutfromup去掉上方和下方空白后的纵坐标
				//float leftH = getCutfromdown(adhleft) - getCutfromup(adhleft) + 1;
				//float rightH = getCutfromdown(adhright) - getCutfromup(adhright) + 1;
				////cout << getCutfromdown(adhright) << " " << getCutfromup(adhright);

				//if ((Max(leftH, rightH)*1.0 /Min(leftH, rightH)*1.0) < Hhrotio)
				//{//切
				//	left[2] = newcom1.at(i)[2]+getCutfromup(adhleft);
				//	left[3] = newcom1.at(i)[2]+ getCutfromdown(adhleft);
				//	right[2] = newcom1.at(i)[2]+ getCutfromup(adhright);
				//	right[3] = newcom1.at(i)[2]+ getCutfromdown(adhright);
				//	newcom2.push_back(left);
				//	newcom2.push_back(right);
				//}
				//else
				//{//不切
				//	newcom2.push_back(newcom1[i]);
				//}

				//方法二：标准差
				//float sd = ((abs(w - meanWidth))*1.0)/ (meanWidth*1.0);

				int *left = new int[4];
				int *right = new int[4];
				left[0] = newcom1.at(i)[0];
				left[1] = traverseCutPoint(adh) + newcom1.at(i)[0];//traverseCutPoint返回最小sum所在列
				left[2] = newcom1.at(i)[2];
				left[3] = newcom1.at(i)[3];
				right[0] = left[1];
				right[1] = newcom1.at(i)[1];
				right[2] = newcom1.at(i)[2];
				right[3] = newcom1.at(i)[3];
				Mat adhleft = src1(Range(left[2], left[3]), Range(left[0], left[1]));
				Mat adhright = src2(Range(right[2], right[3]), Range(right[0], right[1]));
				////计算左右两部分去掉上方空白后的高度
				//getCutfromup去掉上方和下方空白后的纵坐标
				float leftH = getCutfromdown(adhleft) - getCutfromup(adhleft) + 1;
				float rightH = getCutfromdown(adhright) - getCutfromup(adhright) + 1;
				if (sd <= 1.6 || ((Max(leftH, rightH)*1.0 / Min(leftH, rightH)*1.0) >= Hhrotio))
				{//不切
					newcom2.push_back(newcom1[i]);
				}
				else
				{

					left[2] = newcom1.at(i)[2] + getCutfromup(adhleft);
					left[3] = newcom1.at(i)[2] + getCutfromdown(adhleft);
					right[2] = newcom1.at(i)[2] + getCutfromup(adhright);
					right[3] = newcom1.at(i)[2] + getCutfromdown(adhright);

					newcom2.push_back(left);
					newcom2.push_back(right);
				}
			}
			else
			{//至少一条缝，左或右切
				int cutWidth = (lr[0] - lr[1]) >= 0 ? lr[0] - lr[1] : lr[1] - lr[0];
				int cutCenter = (lr[0] + lr[1]) / 2;

				if (cutWidth*1.0 / adh.cols*1.0 > cutWandTotalW)
				{//至少有两条缝
				 //加两条缝的情况
				 //去掉两边，保留中间接着找缝
					Mat cutCenter = src(Range(newcom1.at(i)[2], newcom1.at(i)[3]), Range(newcom1.at(i)[0] + lr[0], newcom1.at(i)[0] + lr[1]));
					vector<int> l2r2 = getCenterCutLAndR(cutCenter);//返回两个间隙所在列
					if (l2r2.size() == 0) {
						int *left = new int[4];
						int *right = new int[4];
						left[0] = newcom1.at(i)[0];
						left[1] = newcom1.at(i)[0] + lr[0];
						left[2] = newcom1.at(i)[2];
						left[3] = newcom1.at(i)[3];
						right[0] = newcom1.at(i)[0] + lr[1];
						right[1] = newcom1.at(i)[1];
						right[2] = newcom1.at(i)[2];
						right[3] = newcom1.at(i)[3];
						newcom2.push_back(left);
						newcom2.push_back(right);
					}
					else {
						if ((l2r2[0] < cutCenter.cols - l2r2[1]) && (sd > 1.6))
						{//左边缝窄切右边
							int *left = new int[4];
							int *right = new int[4];
							left[0] = newcom1.at(i)[0];
							left[1] = newcom1.at(i)[0] + lr[0] + l2r2[1];
							left[2] = newcom1.at(i)[2];
							left[3] = newcom1.at(i)[3];
							right[0] = newcom1.at(i)[0] + lr[1];
							right[1] = newcom1.at(i)[1];
							right[2] = newcom1.at(i)[2];
							right[3] = newcom1.at(i)[3];
							newcom2.push_back(left);
							newcom2.push_back(right);
						}
						if ((l2r2[0] >= cutCenter.cols - l2r2[1]) && (sd > 1.6))
						{//右边窄切左边
							int *left = new int[4];
							int *right = new int[4];
							left[0] = newcom1.at(i)[0];
							left[1] = newcom1.at(i)[0] + lr[0];
							left[2] = newcom1.at(i)[2];
							left[3] = newcom1.at(i)[3];
							right[0] = newcom1.at(i)[0] + lr[0] + l2r2[0];
							right[1] = newcom1.at(i)[1];
							right[2] = newcom1.at(i)[2];
							right[3] = newcom1.at(i)[3];
							newcom2.push_back(left);
							newcom2.push_back(right);
						}
					}

				}
				else
				{//一条缝
					if (cutCenter <= 0.5 * adh.cols)
					{//在左边，确定缝宽
						Mat adhleft;
						Mat adhright;
						if (((cutWidth*1.0 / lr[0] * 1.0) > 0.3) && (sd > 1.6))
						{//左半部分跟前一个字合并合并
							int *left = new int[4];
							if (i == 0)
							{
								left[0] = newcom1[i][0];
								left[1] = newcom1[i][0] + lr[0];
								left[2] = newcom1[i][2];
								left[3] = newcom1[i][3];
								adhleft = src2(Range(left[2], left[3]), Range(left[0], left[1]));
								left[2] = newcom1[i][2] + getCutfromup(adhleft);
								left[3] = newcom1[i][2] + getCutfromdown(adhleft);
							}
							else
							{

								//left[0] = newcom2[newcom2.size() - 1][0];
								left[0] = newcom1[i][0];
								left[1] = newcom1[i][0] + lr[0];
								/*left[2] = Min(newcom2[newcom2.size() - 1][2], newcom1[i][2]);
								left[3] = Max(newcom2[newcom2.size() - 1][3], newcom1[i][3]);*/
								left[2] = newcom1[i][2];
								left[3] = newcom1[i][3];
								//newcom2.pop_back();
							}
							int *right = new int[4];
							right[0] = newcom1[i][0] + lr[1];
							right[1] = newcom1[i][1];
							right[2] = newcom1[i][2];
							right[3] = newcom1[i][3];
							adhright = src2(Range(right[2], right[3]), Range(right[0], right[1]));
							right[2] = newcom1[i][2] + getCutfromup(adhright);
							right[3] = newcom1[i][2] + getCutfromdown(adhright);
							newcom2.push_back(left);
							newcom2.push_back(right);
						}
						else
						{//不切,如左、右半部分/总宽<0.2，接着往左或右找投影最细处切，否则不切
							if ((lr[0] * 1.0 / adh.cols*1.0 < LRwithTotalW) && (sd > 1.6))
							{//切
								Mat adh1_r = src(Range(newcom1.at(i)[2], newcom1.at(i)[3]), Range(newcom1.at(i)[0] + lr[1], newcom1.at(i)[1]));
								//imshow("dd", adh1);
								int *left = new int[4];
								int *right = new int[4];
								left[0] = newcom1.at(i)[0];
								left[1] = traverseCutPoint(adh1_r) + lr[1] + newcom1.at(i)[0];//traverseCutPoint返回最小sum所在列
								left[2] = newcom1.at(i)[2];
								left[3] = newcom1.at(i)[3];
								right[0] = left[1];
								right[1] = newcom1.at(i)[1];
								right[2] = newcom1.at(i)[2];
								right[3] = newcom1.at(i)[3];
								newcom2.push_back(left);
								newcom2.push_back(right);
							}
							else
							{
								newcom2.push_back(newcom1[i]);
							}

						}
					}
					else
					{//在右边，确定缝宽
						Mat adhleft;
						Mat adhright;
						if (((cutWidth*1.0 / (adh.cols - lr[1])*1.0) > 0.3) && (sd > 1.6))
						{//右半部分跟后一个字合并
							int *left = new int[4];
							left[0] = newcom1[i][0];
							left[1] = newcom1[i][0] + lr[0];
							left[2] = newcom1[i][2];
							left[3] = newcom1[i][3];
							adhleft = src2(Range(left[2], left[3]), Range(left[0], left[1]));
							left[2] = newcom1[i][2] + getCutfromup(adhleft);
							left[3] = newcom1[i][2] + getCutfromdown(adhleft);
							newcom2.push_back(left);
							//最后一个字拆开右半部分不需要与后面合并
							int *right = new int[4];
							if (i == newcom1.size() - 1)
							{
								right[0] = newcom1[i][0] + lr[1];
								right[1] = newcom1[i][1];
								right[2] = newcom1[i][2];
								right[3] = newcom1[i][3];
								adhright = src2(Range(right[2], right[3]), Range(right[0], right[1]));
								right[2] = newcom1[i][2] + getCutfromup(adhright);
								right[3] = newcom1[i][2] + getCutfromdown(adhright);
								newcom2.push_back(right);
								break;
							}
							else
							{
								right[0] = newcom1[i][0] + lr[1];
								//right[1] = newcom1[i + 1][1];
								right[1] = newcom1[i][1];
								//right[2] = Min(newcom1[i][2], newcom1[i + 1][2]);
								//right[3] = Max(newcom1[i][3], newcom1[i + 1][3]);
								right[2] = newcom1[i][2];
								right[3] = newcom1[i][3];
								newcom2.push_back(right);
								//++count3;
							}

						}
						else
						{
							if (((adh.cols - lr[1])*1.0 / adh.cols*1.0 < LRwithTotalW) && (sd > 1.6))
							{//向左找
								Mat adh1_l = src(Range(newcom1.at(i)[2], newcom1.at(i)[3]), Range(newcom1.at(i)[0], newcom1.at(i)[0] + lr[0]));
								//imshow("dd1", adh1);
								int *left = new int[4];
								int *right = new int[4];
								left[0] = newcom1.at(i)[0];
								left[1] = traverseCutPoint(adh1_l) + newcom1.at(i)[0];//traverseCutPoint返回最小sum所在列
								left[2] = newcom1.at(i)[2];
								left[3] = newcom1.at(i)[3];
								right[0] = left[1];
								right[1] = newcom1.at(i)[1];
								right[2] = newcom1.at(i)[2];
								right[3] = newcom1.at(i)[3];
								newcom2.push_back(left);
								newcom2.push_back(right);
							}
							else
							{
								newcom2.push_back(newcom1[i]);
							}
						}
					}
				}

			}
		}
		else
		{//小于1.2的不用切，直接加
			newcom2.push_back(newcom1[i]);
		}
	}

	for (int i = 0; i < newcom2.size(); i++) {
		float thinrotio = (newcom2[i][1] - newcom2[i][0]) * 1.0 / (newcom2[i][3] - newcom2[i][2]);
		int *r = new int[4];
		r[0] = newcom2[i][0];
		r[1] = newcom2[i][1];
		r[2] = newcom2[i][2];
		r[3] = newcom2[i][3];
		if (thinrotio <= maxthinrotio) {
			if (i == 0) {
				int x1 = r[0];
				int x2 = newcom2.at(i + 1)[1];
				int y1;
				int y2;
				y1 = Min(r[2], newcom2.at(i + 1)[2]);
				y2 = Max(r[3], newcom2.at(i + 1)[3]);
				r[0] = x1;
				r[1] = x2;
				r[2] = y1;
				r[3] = y2;
				newcom3.push_back(r);
				i++;
			}
			else if (i == (newcom2.size() - 1))
			{
				int x1 = newcom3[newcom3.size() - 1][0];
				int x2 = r[1];
				int y1;
				int y2;
				y1 = Min(newcom3[newcom3.size() - 1][2], r[2]);
				y2 = Max(newcom3[newcom3.size() - 1][3], r[3]);
				r[0] = x1;
				r[1] = x2;
				r[2] = y1;
				r[3] = y2;
				newcom3.pop_back();
				newcom3.push_back(r);
			}
			else
			{
				float cutLW, cutRW;
				cutLW = r[0] - newcom3[newcom3.size() - 1][1];
				cutRW = newcom2[i + 1][0] - r[1];
				if (cutLW >= cutRW)
				{//向右合并

					r[0] = r[0];
					r[1] = newcom2[i + 1][1];
					r[2] = Min(newcom2[i + 1][2], r[2]);
					r[3] = Max(newcom2[i + 1][3], r[3]);
					newcom3.push_back(r);
					i++;
				}
				if (cutLW < cutRW)
				{//向左合并
					int tempx = newcom3[newcom3.size() - 1][0];//与当前newcom1中最后一个合并
					int tempy1 = newcom3[newcom3.size() - 1][2];
					int tempy2 = newcom3[newcom3.size() - 1][3];
					r[0] = tempx;
					r[1] = r[1];
					r[2] = Min(tempy1, r[2]);
					r[3] = Max(tempy2, r[3]);
					newcom3.pop_back();
					newcom3.push_back(r);
				}
			}
		}
		else {
			newcom3.push_back(r);
		}
	}
	return newcom3;
}

vector<int*> morecut(Mat src) {
	vector<vector<Point>> contours;
	findContours(src,
		contours,
		CV_RETR_EXTERNAL,
		//CV_RETR_TREE,
		CV_CHAIN_APPROX_NONE);
	Mat contoursImg = Mat(src.rows, src.cols, CV_8U);
	drawContours(contoursImg, contours);
	vector<int*> coordinates = CCPoints2Coordinates(contours);
	sort(coordinates.begin(), coordinates.end(), coordinateCmp);
	vector<int*> combined = combineOverlapComponents(coordinates);
	adjustCombinedComponent(combined);
	int LH = estimateComponentsMean(combined);
	vector<int*> potentialPT = detectPotentialPT(combined, LH, 0.8, 1.0);
	vector<vector<int*>> singleStrokes = detectSingleStroke(src, potentialPT, LH * 0.25, 1);
	vector<int*> split = splitBySingleStroke(combined, singleStrokes);
	/*for (int i = 0; i < split.size(); i++) {
	rectangle(src, Point(split.at(i)[0], split.at(i)[2]), Point(split.at(i)[1], split.at(i)[3]), Scalar(0, 0, 0), 0.2);
	}
	imshow("split", src);
	waitKey(0);*/
	return split;
}

void saverec(vector<int*> split, string fileName, char* Savedir, Mat fgImg) {
	Mat *fgroi = new Mat[split.size()];
	char save_file[200];
	//char *Savdir = (char*)output.data();
	//mkdir(Savdir);
	char *filename = (char*)fileName.data();
	char *p = strrchr(filename, '\\');
	string ps(p);
	string nn = ps.substr(1, ps.length() - 5);
	int a = atoi(nn.c_str());
	string nn1 = std::to_string(a);
	string dir2 = "分割\\" + nn1 + "\\";
	char *S = (char*)dir2.data();
	strcat(Savedir, S);
	FILE *fp = NULL;
	fp = fopen(Savedir, "w");
	if (!fp)
		mkdir(Savedir);
	for (int i = 0; i < (split.size()); i++) {
		//cout <<i<<":"<< "左：" << split.at(i)[0] << " 右：" << split.at(i)[1] << " 上：" << split.at(i)[2] << " 下：" << split.at(i)[3] << endl;
		//cout << i << ":" << "宽："<<fgImg.cols <<"高："<< fgImg.rows << endl;
		fgroi[i] = fgImg(Range(split.at(i)[2], split.at(i)[3]), Range(split.at(i)[0], split.at(i)[1]));
		/*if (!((fgroi[i].rows < 10) || (fgroi[i].cols < 10))) {*/
		sprintf(save_file, "%s%d.jpg", Savedir, i);
		bitwise_not(fgroi[i], fgroi[i]);
		imwrite(save_file, fgroi[i]);
		//}
	}
}

vector<int> maptolookcut(IplImage *img) {
	//int*summ3 = new int[10000];
	vector<int> fg;
	int pre = 0;
	int next = 0;
	int width1 = img->width;
	int height1 = img->height;
	uchar *ptr1 = (uchar *)img->imageData;
	for (int i = 0; i < width1; i++) {
		for (int j = 0; j < height1; j++) {
			summ9[i] += (int)ptr1[j*img->widthStep + i];
		}
		if (i == 0)
			pre = summ9[i];
		else
		{
			pre = summ9[i - 1];
			next = summ9[i];
		}
		if ((pre > 0) && (next == 0)) {
			fg.push_back(i);
		}
		if ((pre == 0) && (next > 0)) {
			fg.push_back(i - 1);
		}
	}
	fg.push_back(width1);
	memset(summ9, 0, 10000);
	return fg;
}

cv::Mat thinImage(cv::Mat & src, const int maxIterations = -1)
{
	assert(src.type() == CV_8UC1);
	cv::Mat dst;
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	int count = 0;  //记录迭代次数    
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达    
			break;
		std::vector<uchar *> mFlag; //用于标记需要删除的点    
									//对点标记    
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记    
				//  p9 p2 p3    
				//  p8 p1 p4    
				//  p7 p6 p5    
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
					{
						//标记    
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除    
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束    
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空    
		}

		//对点标记    
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记    
				//  p9 p2 p3    
				//  p8 p1 p4    
				//  p7 p6 p5    
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
					{
						//标记    
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除    
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束    
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空    
		}
	}
	return dst;
}

void filterOver(Mat thinSrc)
{
	assert(thinSrc.type() == CV_8UC1);
	int width = thinSrc.cols;
	int height = thinSrc.rows;
	for (int i = 0; i < height; ++i)
	{
		uchar * p = thinSrc.ptr<uchar>(i);
		for (int j = 0; j < width; ++j)
		{
			// 实现两个点之间至少隔一个像素  
			//  p9 p2 p3    
			//  p8 p1 p4    
			//  p7 p6 p5    
			uchar p1 = p[j];
			if (p1 != 1) continue;
			uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
			uchar p8 = (j == 0) ? 0 : *(p + j - 1);
			uchar p2 = (i == 0) ? 0 : *(p - thinSrc.step + j);
			uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - thinSrc.step + j + 1);
			uchar p9 = (i == 0 || j == 0) ? 0 : *(p - thinSrc.step + j - 1);
			uchar p6 = (i == height - 1) ? 0 : *(p + thinSrc.step + j);
			uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + thinSrc.step + j + 1);
			uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + thinSrc.step + j - 1);
			if (p2 + p3 + p8 + p9 >= 1)
			//if (p2 + p8  >= 1)
			{
				p[j] = 0;
			}
		}
	}
}

vector<Point> getPoints(Mat thinSrc, int raudis = 4, int thresholdMax = 6, int thresholdMin = 4)
{
	assert(thinSrc.type() == CV_8UC1);
	int width = thinSrc.cols;
	int height = thinSrc.rows;
	cv::Mat tmp;
	thinSrc.copyTo(tmp);
	std::vector<cv::Point> points;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (*(tmp.data + tmp.step * i + j) == 0)
			{
				continue;
			}
			int count = 0;
			for (int k = i - raudis; k < i + raudis + 1; k++)
			{
				for (int l = j - raudis; l < j + raudis + 1; l++)
				{
					if (k < 0 || l < 0 || k>height - 1 || l>width - 1)
					{
						continue;

					}
					else if (*(tmp.data + tmp.step * k + l) == 1)
					{
						count++;
					}
				}
			}

			if (count > thresholdMax/* || count<thresholdMin*/)
			{
				Point point(j, i);
				points.push_back(point);
			}
		}
	}
	return points;
}

Mat getthinImage(Mat src) {
	cvtColor(src, src, CV_BGR2GRAY);
	bitwise_not(src, src);
	cv::threshold(src, src, 128, 1, cv::THRESH_BINARY);
	cv::Mat dst = thinImage(src, -1);
	/*dst = dst * 255;
	src = src * 255;*/
	return dst;
}

bool SingleCut(Mat src, int pos) {
	int count = 0;
	int height = src.rows;
	int pre = 0;
	int next = 0;
	for (int i = 0; i < height; i++) {
		if (i == 0)
			pre = (int)src.at<uchar>(i, pos);
		else
		{
			pre = (int)src.at<uchar>(i - 1, pos);
			next = (int)src.at<uchar>(i, pos);
		}
		if ((pre > 0) && (next == 0)) {
			count++;
		}
		if ((pre == 0) && (next > 0)) {
			count++;
		}
	}
	if (count >= 3) {
		return false;
	}
	else
	{
		return true;
	}
}

int MapsumtoCut(Mat img) {
	int result = 0;
	int col = img.cols;
	int row = img.rows;
	int *sum = new int[col];
	for (int i = 0; i < col; i++) {
		int ss = 0;
		for (int j = 0; j < row; j++) {
			ss += ((int)img.at<uchar>(j, i) / 255);
		}
		sum[i] = ss;
	}
	int leftmax = 0;
	int rightmax = 0;
	int leftmaxindex, rightmaxindex = 0;
	for (int i = 0; i < col / 2; i++) {
		if (sum[i] >= leftmax) {
			leftmax = sum[i];
			leftmaxindex = i;
		}
	}
	for (int i = col - 1; i >= col / 2; i--) {
		if (sum[i] >= rightmax) {
			rightmax = sum[i];
			rightmaxindex = i;
		}
	}
	//int minthreesum = 9999;		//滑动找最小
	//int *three = new int[3];
	//for (int i = leftmaxindex; i < rightmaxindex - 1; i++) {
	//	int threesum = sum[i] + sum[i + 1] + sum[i + 2];
	//	if (threesum < minthreesum) {
	//		minthreesum = threesum;
	//		three[0] = i;
	//		three[1] = i + 1;
	//		three[2] = i + 2;
	//	}
	//}
	//if (sum[three[0]] < sum[three[1]]) {
	//	result = three[0];
	//}
	//else
	//{
	//	result = three[1];
	//}
	//if (sum[result] > sum[three[2]]) {
	//	result = three[2];
	//}

	int minthreesum = 9999;		//直接找最小
	int m = 0;
	for (int i = leftmaxindex; i < rightmaxindex + 1; i++) {
		if (sum[i] < minthreesum) {
			minthreesum = sum[i];
			m = i;
		}
	}
	int twomax = Max(leftmax, rightmax);
	/*int twomin = Min(leftmax, rightmax);
	float cc = twomin / twomax;
	if (cc < 0.6) {
		return 0;
	}*/
	float bb = twomax * 1.0 / 1.5;
	int aa = bb;
	if (minthreesum < aa) {
		result = m;
	}
	return result;
}

int Mapsumtocut_with_range(Mat img, int* range) {
	int result = 0;
	int col = img.cols;
	int row = img.rows;
	int *sum = new int[col];
	for (int i = 0; i < col; i++) {
		int ss = 0;
		for (int j = 0; j < row; j++) {
			ss += ((int)img.at<uchar>(j, i) / 255);
		}
		sum[i] = ss;
	}
	int leftmax = 0;
	int rightmax = 0;
	int leftmaxindex, rightmaxindex = 0;
	for (int i = 0; i < col / 2; i++) {
		if (sum[i] >= leftmax) {
			leftmax = sum[i];
			leftmaxindex = i;
		}
	}
	for (int i = col - 1; i >= col / 2; i--) {
		if (sum[i] >= rightmax) {
			rightmax = sum[i];
			rightmaxindex = i;
		}
	}
	int minthreesum = 9999;		//直接找最小
	int m = 0;
	for (int i = leftmaxindex; i < rightmaxindex + 1; i++) {
		if (sum[i] < minthreesum) {
			minthreesum = sum[i];
			m = i;
		}
	}
	int twomax = Max(leftmax, rightmax);
	float bb = twomax * 1.0 / 3.0;
	int aa = bb;
	if (minthreesum < aa) {
		result = m;
	}
	for (int i = m;; i--) {
		if (sum[i] > minthreesum * 1.5) {
			range[0] = i + 1;
			break;
		}
	}
	for (int i = m;; i++) {
		if (sum[i] > minthreesum * 1.5) {
			range[1] = i - 1;
			break;
		}
	}
	return result;
}

int MapsumtoCuty(Mat img) {
	int result = 0;
	int col = img.cols;
	int row = img.rows;
	int *sum = new int[row];
	int maxsum = -9999;
	int minsum = 9999;
	for (int i = 0; i < row; i++) {
		int ss = 0;
		for (int j = 0; j < col; j++) {
			ss += ((int)img.at<uchar>(i, j) / 255);
		}
		sum[i] = ss;
		if (sum[i] > maxsum) {
			maxsum = sum[i];
		}
		if (sum[i] < minsum) {
			minsum = sum[i];
		}
	}
	int mid = (minsum + maxsum) / 2;
	//int mid = maxsum * 0.7;
	int flag = 0;
	for (int i = 1; i < row; i++) {
		if ((sum[i] > mid) && (sum[i - 1] < mid)) {
			flag++;
		}
		if ((sum[i] < mid) && (sum[i - 1] > mid)) {
			flag++;
		}
	}
	
	return flag;
}

int Crest(Mat src) {
	int col = src.cols;
	int row = src.rows;
	int *sum = new int[col];
	int maxpixel = -9999;
	for (int i = 0; i < col; i++) {
		int ss = 0;
		for (int j = 0; j < row; j++) {
			ss += ((int)src.at<uchar>(j, i) / 255);
		}
		sum[i] = ss;
		if (sum[i] > maxpixel) {
			maxpixel = sum[i];
		}
	}
	int norm = maxpixel * 0.99;
	int count = 0;
	for (int i = 1; i < col - 1; i++) {
		if ((sum[i - 1] < norm) && (sum[i + 1] > norm))
			count += 1;
		if ((sum[i - 1] > norm) && (sum[i + 1] < norm))
			count += 1;
	}
	return count / 2;
}

vector<vector<Point>> removeShortContoursfornumber(vector<vector<Point>>& contours, int rows) {
	//vector<int*> consize;
	//vector<int> area;
	//for (int i = 0; i < contours.size(); i++) {
	//	int* a = new int[2];
	//	a[0] = i;
	//	a[1] = contours[i].size();
	//	a[1] = contourArea(contours[i]);
	//	area.push_back(a[1]);
	//	consize.push_back(a);
	//}
	//sort(area.begin(), area.end(), widthCmp);
	//sort(consize.begin(), consize.end(), contoursizeCmp);
	//vector<vector<Point>> result;
	//int presize = consize[0][1];
	//result.push_back(contours[consize[0][0]]);
	//bool flag = false;
	//for (int i = 1; i < consize.size(); i++) {
	//	if (!flag) {
	//		presize = consize[i - 1][1];
	//		float rotio = presize * 1.0 / consize[i][1] * 1.0;

	//		if (rotio < 0.2) {
	//			//result.pop_back();
	//			result.clear();
	//			result.push_back(contours[consize[i][0]]);
	//			flag = true;
	//		}
	//		else
	//		{
	//			result.push_back(contours[consize[i][0]]);
	//		}
	//	}
	//	else {
	//		result.push_back(contours[consize[i][0]]);
	//	}
	//}
	//return result;

	vector<vector<Point>> result;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if ((contours[i][j].y > rows * 0.12) && (contours[i][j].y < rows * 0.88)) {
				result.push_back(contours[i]);
				break;
			}
		}
	}
	return result;
}

vector<vector<Point>> removeShortContoursfornumber1(vector<vector<Point>>& contours, int rows) {
	vector<vector<Point>> result;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size() - 1; j++) {
			if ((contours[i][j].y > rows * 0.5) && (contours[i][j + 1].y < rows * 0.5)) {
				result.push_back(contours[i]);
				break;
			}
			if ((contours[i][j].y < rows * 0.5) && (contours[i][j + 1].y > rows * 0.5)) {
				result.push_back(contours[i]);
				break;
			}
		}
	}
	return result;
}

vector<int*> getneighbor(Mat src, int x, int y) {
	vector<int*> neighbor;
	if (((x - 1) >= 0) && (src.at<uchar>(x - 1, y) == 255)) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && ((y - 1) >= 0) && (src.at<uchar>(x - 1, y - 1) == 255)) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && src.at<uchar>(x - 1, y + 1) == 255) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if (((y - 1) >= 0) && (src.at<uchar>(x, y - 1) == 255)) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x, y + 1) == 255) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 1, y) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((y - 1) >= 0) && (src.at<uchar>(x + 1, y - 1) == 255)) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 1, y + 1) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	return neighbor;
}

vector<int*> getneighbor_2nei(Mat src, int x, int y) {
	vector<int*> neighbor;
	if (((x - 2) >= 0) && (src.at<uchar>(x - 2, y) == 255)) {
		int* a = new int[2];
		a[0] = x - 2;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && ((y - 1) >= 0) && (src.at<uchar>(x - 1, y - 1) == 255)) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && src.at<uchar>(x - 1, y + 1) == 255) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if (((y - 2) >= 0) && (src.at<uchar>(x, y - 2) == 255)) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y - 2;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x, y + 2) == 255) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y + 2;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 2, y) == 255) {
		int* a = new int[2];
		a[0] = x + 2;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((y - 1) >= 0) && (src.at<uchar>(x + 1, y - 1) == 255)) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 1, y + 1) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y + 1;
		neighbor.push_back(a);
	}

	if (((x - 2) >= 0) && ((y - 2) >= 0) && (src.at<uchar>(x - 2, y - 2) == 255)) {
		int* a = new int[2];
		a[0] = x - 2;
		a[1] = y - 2;
		neighbor.push_back(a);
	}
	if (((x - 2) >= 0) && src.at<uchar>(x - 2, y + 2) == 255) {
		int* a = new int[2];
		a[0] = x - 2;
		a[1] = y + 2;
		neighbor.push_back(a);
	}
	if (((y - 2) >= 0) && (src.at<uchar>(x + 2, y - 2) == 255)) {
		int* a = new int[2];
		a[0] = x + 2;
		a[1] = y - 2;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 2, y + 2) == 255) {
		int* a = new int[2];
		a[0] = x + 2;
		a[1] = y + 2;
		neighbor.push_back(a);
	}

	if (((x - 2) >= 0) && (src.at<uchar>(x - 2, y + 1) == 255)) {
		int* a = new int[2];
		a[0] = x - 2;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if ((src.at<uchar>(x, y + 1) == 255)) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if ((src.at<uchar>(x + 2, y + 1) == 255)) {
		int* a = new int[2];
		a[0] = x + 2;
		a[1] = y + 1;
		neighbor.push_back(a);
	}
	if (((y - 1) >= 0) && src.at<uchar>(x, y - 1) == 255) {
		int* a = new int[2];
		a[0] = x;
		a[1] = y - 1;
		neighbor.push_back(a);
	}

	if (((x - 1) >= 0) && src.at<uchar>(x - 1, y) == 255) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((x + 1) >= 0) && src.at<uchar>(x + 1, y) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && src.at<uchar>(x - 1, y + 2) == 255) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y + 2;
		neighbor.push_back(a);
	}
	if (src.at<uchar>(x + 1, y + 2) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y + 2;
		neighbor.push_back(a);
	}
	if (((x - 2) >= 0) && ((y - 1) >= 0) && src.at<uchar>(x - 2, y - 1) == 255) {
		int* a = new int[2];
		a[0] = x - 2;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (((y - 1) >= 0) && src.at<uchar>(x + 2, y - 1) == 255) {
		int* a = new int[2];
		a[0] = x + 2;
		a[1] = y - 1;
		neighbor.push_back(a);
	}
	if (((x - 1) >= 0) && ((y - 2) >= 0) && src.at<uchar>(x - 1, y - 2) == 255) {
		int* a = new int[2];
		a[0] = x - 1;
		a[1] = y - 2;
		neighbor.push_back(a);
	}
	if (((y - 2) >= 0) && src.at<uchar>(x + 1, y - 2) == 255) {
		int* a = new int[2];
		a[0] = x + 1;
		a[1] = y - 2;
		neighbor.push_back(a);
	}
	return neighbor;
}

vector<int*> filterupneighbor(vector<int*> nei, int x) {
	vector<int*> result;
	for (int i = 0; i < nei.size(); i++) {
		if (nei[i][0] > x) {
			result.push_back(nei[i]);
		}
	}
	return result;
}

bool existin_passed_points(int* point, vector<int*> cutpath) {
	bool flag = false;
	for (int i = cutpath.size() - 1; i >= 0; i--) {
		if (point[0] == cutpath[i][0] && point[1] == cutpath[i][1]) {
			flag = true;
			break;
		}
	}
	return flag;
}

bool isrealcross_point(int* point, vector<cv::Point> points) {
	bool flag = false;
	for (int i = 0; i < points.size(); i++) {
		if (point[0] == points[i].y && point[1] == points[i].x) {
			flag = true;
			break;
		}
	}
	return flag;
}

int* getmost_middle_point(vector<int*> coord, int mid) {
	int* result = new int[2];
	int m = 99999;
	sort(coord.begin(), coord.end(), coordinateCmp);
	int h = coord[0][0];
	for (int i = 0; i < coord.size(); i++) {
		if ((coord[i][0] == h)) {
			if (abs(mid - coord[i][1]) < m) {
				result[0] = coord[i][0];
				result[1] = coord[i][1];
				m = abs(mid - coord[i][1]);
			}
		}
		else {
			break;
		}
	}
	return result;
}

int* gethighestpoint_x(vector<int*> coord, int ly, int ry) {
	int* result = new int[2];
	vector<int> left;
	vector<int> right;
	sort(coord.begin(), coord.end(), contoursizeCmp);
	for (int i = 0; i < coord.size(); i++) {
		if (coord[i][1] == ly) {
			left.push_back(coord[i][0]);
		}
		if (coord[i][1] > ly) {
			break;
		}
	}

	for (int i = coord.size() - 1; i >= 0; i--) {
		if (coord[i][1] == ry) {
			right.push_back(coord[i][0]);
		}
		if (coord[i][1] < ry) {
			break;
		}
	}
	sort(left.begin(), left.end(), widthCmp);
	sort(right.begin(), right.end(), widthCmp);
	result[0] = left[0];
	result[1] = right[0];
	return result;
}