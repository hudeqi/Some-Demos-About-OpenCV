#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#include "cutcol.h"
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include "opencv2\contrib\contrib.hpp"
#include <io.h>
using namespace std;
using namespace cv;

int main_bgfb() {
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\扫描图片\\扫描图片\\img088.jpg");
	flip(src, src, 0);
	flip(src, src, 1);
	imwrite("C:\\Users\\Administrator\\Desktop\\kk.png", src);
	return 0;
}



//void getAll_cutpath(vector<vector<int*>>& resultpath, Mat dst, Mat tc_up, bool up) {				//基本完成功能
//	vector<int*> backcoord;
//	for (int i = 0; i < dst.rows; i++)
//		for (int j = 0; j < dst.cols; j++) {
//			if ((int)dst.at<uchar>(i, j) == 255) {
//				int* a = new int[2];
//				a[0] = i;
//				a[1] = j;
//				backcoord.push_back(a);
//			}
//		}
//	sort(backcoord.begin(), backcoord.end(), contoursizeCmp);
//	int left = backcoord[0][1] * 0.85 + 0.15 * backcoord[backcoord.size() - 1][1];
//	int right = backcoord[backcoord.size() - 1][1] * 0.85 + 0.15 * backcoord[0][1];
//	int lefth = gethighestpoint_x(backcoord, left, right)[0];
//	int righth = gethighestpoint_x(backcoord, left, right)[1];
//	int tempx = lefth;
//	int tempy = left;
//	/*line(dst, Point(left - 2, 0), Point(left - 2, lefth + 5), Scalar(0));
//	line(dst, Point(right + 2, 0), Point(right + 2, righth + 5), Scalar(0));*/
//	line(dst, Point(left - 2, 0), Point(left - 2, dst.rows - 1), Scalar(0));
//	line(dst, Point(right + 2, 0), Point(right + 2, dst.rows - 1), Scalar(0));
//
//	vector<vector<int*>> temppathlist;
//	vector<int*> temp;
//	int* next;
//	vector<pair<int*, vector<int*>>> tolookpoint;
//	vector<int*> cutpath;
//	vector<int*> passed_allpoints;
//	int* a = new int[2];
//	a[0] = tempx;
//	a[1] = tempy;
//	cutpath.push_back(a);
//	passed_allpoints.push_back(a);
//	vector<int*> neighbor = getneighbor(dst, tempx, tempy);
//	vector<int*> filter_neighbor;
//	while (neighbor.size() != 1) {
//		for (int n = 0; n < neighbor.size(); n++) {
//			if (!(existin_passed_points(neighbor[n], passed_allpoints))) {
//				filter_neighbor.push_back(neighbor[n]);
//			}
//		}
//		if (filter_neighbor.size() <= 1) {
//			if (filter_neighbor.size() == 1) {
//				tempx = filter_neighbor[0][0];
//				tempy = filter_neighbor[0][1];
//				int* a = new int[2];
//				a[0] = tempx;
//				a[1] = tempy;
//				cutpath.push_back(a);
//				passed_allpoints.push_back(a);
//			}
//			else {
//				break;
//			}
//		lookdown:
//			neighbor = getneighbor(dst, tempx, tempy);
//
//		}
//		else if (filter_neighbor.size() > 1) {
//			int* crosspoint = new int[2];
//			crosspoint[0] = tempx;
//			crosspoint[1] = tempy;
//			tolookpoint.push_back(make_pair(crosspoint, filter_neighbor));
//			for (int j = 0; j < filter_neighbor.size() - 1; j++) {
//				vector<int*> newpath;
//				for (int i = 0; i < cutpath.size(); i++) {
//					newpath.push_back(cutpath[i]);
//				}
//				temppathlist.push_back(newpath);
//			}
//		nextpoint:
//			temp = tolookpoint[tolookpoint.size() - 1].second;
//		nexttemp:
//			while (temp.size() > 0) {
//				next = temp[temp.size() - 1];
//				temp.pop_back();
//				tolookpoint[tolookpoint.size() - 1].second.pop_back();
//				tempx = next[0];
//				tempy = next[1];
//				int* a = new int[2];
//				a[0] = tempx;
//				a[1] = tempy;
//				cutpath.push_back(a);
//				passed_allpoints.push_back(a);
//
//				Mat tt = tc_up.clone();
//				for (int m = 0; m < cutpath.size(); m++) {
//					tt.at<uchar>(cutpath[m][0], cutpath[m][1]) = 255;
//				}
//				imshow("fc", tt);
//				waitKey();
//				goto lookdown;
//			}
//			temppathlist.pop_back();
//			cutpath.clear();
//			if (temppathlist.size() == 0) {
//				break;
//			}
//			else {
//				cutpath = temppathlist[temppathlist.size() - 1];
//			}
//			tolookpoint.pop_back();
//			if (tolookpoint.size() > 0)
//				goto nextpoint;
//		}
//		filter_neighbor.clear();
//	}
//	if (cutpath.size() > 0) {
//		resultpath.push_back(cutpath);
//	}
//	imshow("1", dst);
//	if (tolookpoint.size() > 0) {
//		Mat temppic = tc_up.clone();
//		for (int j = 0; j < cutpath.size(); j++) {
//			temppic.at<uchar>(cutpath[j][0], cutpath[j][1]) = 255;
//		}
//		imshow("2", temppic);
//		waitKey();
//		cutpath.clear();
//		if (temppathlist.size() > 0) {
//			cutpath = temppathlist[temppathlist.size() - 1];
//			goto nexttemp;
//		}
//	}
//}
//
//int main() {
//	Mat src = imread("C:\\Users\\Administrator\\Desktop\\123\\fghfh.png");
//	cvtColor(src, src, CV_BGR2GRAY);
//	Mat srcclone = src.clone();
//
//	Mat tempsrc = src.clone();					//去掉圈内背景的背景骨架
//	threshold(tempsrc, tempsrc, 0, 255, CV_THRESH_OTSU);
//	copyMakeBorder(tempsrc, tempsrc, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));
//	Mat tc_up = tempsrc.clone();
//	Mat tc_down = tempsrc.clone();
//	Mat tc_result = tempsrc.clone();
//	flip(tc_down, tc_down, 0);
//	flip(tc_down, tc_down, 1);
//	IplImage *img = &IplImage(tempsrc);
//	CvMemStorage * storage = cvCreateMemStorage(0);
//	CvSeq *contours = 0;
//	cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//	cvDrawContours(img, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);
//	Mat s1(img);
//	bitwise_not(s1, s1);
//	cv::threshold(s1, s1, 128, 1, cv::THRESH_BINARY);
//	Mat dst = thinImage(s1, -1);
//	Mat dstclone_for_crosspoint = dst.clone();
//	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 0);
//	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 1);
//	dst = dst * 255;
//	dstclone_for_crosspoint = dstclone_for_crosspoint * 255;
//	
//	vector<vector<int*>> resultpath_up;
//	vector<vector<int*>> resultpath_down;
//	getAll_cutpath(resultpath_up, dst, tc_up, true);
//	getAll_cutpath(resultpath_down, dstclone_for_crosspoint, tc_down, false);
//
//	for (int i = 0; i < resultpath_down.size(); i++) {
//		//for (int j = 0; j < resultpath_down[i].size(); j++) {
//			resultpath_down[i][resultpath_down[i].size() - 1][0] = dst.rows - 1 - resultpath_down[i][resultpath_down[i].size() - 1][0];
//			resultpath_down[i][resultpath_down[i].size() - 1][1] = dst.cols - 1 - resultpath_down[i][resultpath_down[i].size() - 1][1];
//		//}
//	}
//	int result_uppath_id = 0;
//	int result_downpath_id = 0;
//	int mindistance = 9999999;
//	for (int i = 0; i < resultpath_up.size(); i++) {
//		for (int j = 0; j < resultpath_down.size(); j++) {
//			int distance = pow(resultpath_up[i][resultpath_up[i].size() - 1][0] - resultpath_down[j][resultpath_down[j].size() - 1][0], 2)
//				+ pow(resultpath_up[i][resultpath_up[i].size() - 1][1] - resultpath_down[j][resultpath_down[j].size() - 1][1], 2);
//			if (distance < mindistance) {
//				mindistance = distance;
//				result_uppath_id = i;
//				result_downpath_id = j;
//			}
//		}
//	}
//	for (int j = 0; j < resultpath_down[result_downpath_id].size() - 1; j++) {
//		resultpath_down[result_downpath_id][j][0] = dst.rows - 1 - resultpath_down[result_downpath_id][j][0];
//		resultpath_down[result_downpath_id][j][1] = dst.cols - 1 - resultpath_down[result_downpath_id][j][1];
//	}
//	for (int i = 0; i < resultpath_up[result_uppath_id].size(); i++) {
//		tc_result.at<uchar>(resultpath_up[result_uppath_id][i][0], resultpath_up[result_uppath_id][i][1]) = 255;
//	}
//	imshow("vfdvgb2", tc_result);
//	waitKey();
//	for (int i = 0; i < resultpath_down[result_downpath_id].size(); i++) {
//		tc_result.at<uchar>(resultpath_down[result_downpath_id][i][0], resultpath_down[result_downpath_id][i][1]) = 255;
//	}
//	imshow("vfdvgb3", tc_result);
//	waitKey();
//	line(tc_result, Point(resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][1], 
//		resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][0]), 
//		Point(resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][1],
//			resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][0]), Scalar(255));
//	imshow("vfdvgb4", tc_result);
//	waitKey();
//	/*line(tc_result, Point(resultpath_up[result_uppath_id][0][1], 0), Point(resultpath_up[result_uppath_id][0][1], resultpath_up[result_uppath_id][0][0]), Scalar(255));
//	imshow("vfdvgb5", tc_result);
//	waitKey();
//	line(tc_result, Point(resultpath_down[result_downpath_id][0][1], tc_result.rows - 1), Point(resultpath_down[result_downpath_id][0][1], resultpath_down[result_downpath_id][0][0]), Scalar(255));
//	imshow("vfdvgb", tc_result);
//	waitKey();*/
//	return 0;
//}















//void getAll_cutpath(vector<vector<int*>>& resultpath, Mat dst, Mat tc_up, bool up) {		//搜索路径bug已修改，连接条件为上下路径末端最近
//	vector<int*> backcoord;
//	for (int i = 0; i < dst.rows; i++)
//		for (int j = 0; j < dst.cols; j++) {
//			if ((int)dst.at<uchar>(i, j) == 255) {
//				int* a = new int[2];
//				a[0] = i;
//				a[1] = j;
//				backcoord.push_back(a);
//			}
//		}
//	sort(backcoord.begin(), backcoord.end(), contoursizeCmp);
//	int left = backcoord[0][1] * 0.85 + 0.15 * backcoord[backcoord.size() - 1][1];
//	int right = backcoord[backcoord.size() - 1][1] * 0.85 + 0.15 * backcoord[0][1];
//	int lefth = gethighestpoint_x(backcoord, left, right)[0];
//	int righth = gethighestpoint_x(backcoord, left, right)[1];
//	int tempx = lefth;
//	int tempy = left;
//	line(dst, Point(left - 2, 0), Point(left - 2, dst.rows - 1), Scalar(0));
//	line(dst, Point(right + 2, 0), Point(right + 2, dst.rows - 1), Scalar(0));
//	/*line(dst, Point(left - 2, 0), Point(left - 2, lefth + 5), Scalar(0));
//	line(dst, Point(right + 2, 0), Point(right + 2, righth + 5), Scalar(0));
//	dst.at<uchar>(lefth + 5, left - 2) = 255;
//	dst.at<uchar>(righth + 5, right + 2) = 255;*/
//
//	vector<vector<int*>> temppathlist;
//	vector<int*> temp;
//	int* next;
//	vector<pair<int*, vector<int*>>> tolookpoint;
//	vector<int*> cutpath;
//	vector<int*> passed_allpoints;
//	vector<int> temp_all_num;
//	int* a = new int[2];
//	a[0] = tempx;
//	a[1] = tempy;
//	cutpath.push_back(a);
//	passed_allpoints.push_back(a);
//	vector<int*> neighbor = getneighbor(dst, tempx, tempy);
//	vector<int*> filter_neighbor;
//	while (neighbor.size() != 1) {
//		for (int n = 0; n < neighbor.size(); n++) {
//			if (!(existin_passed_points(neighbor[n], passed_allpoints))) {
//				filter_neighbor.push_back(neighbor[n]);
//			}
//		}
//		if (filter_neighbor.size() <= 1) {
//			if (filter_neighbor.size() == 1) {
//				tempx = filter_neighbor[0][0];
//				tempy = filter_neighbor[0][1];
//				int* a = new int[2];
//				a[0] = tempx;
//				a[1] = tempy;
//				cutpath.push_back(a);
//				passed_allpoints.push_back(a);
//			}
//			else {
//				break;
//			}
//		lookdown:
//			neighbor = getneighbor(dst, tempx, tempy);
//
//		}
//		else if (filter_neighbor.size() > 1) {
//			int* crosspoint = new int[2];
//			crosspoint[0] = tempx;
//			crosspoint[1] = tempy;
//			tolookpoint.push_back(make_pair(crosspoint, filter_neighbor));
//			temp_all_num.push_back(filter_neighbor.size());
//			for (int j = 0; j < filter_neighbor.size() - 1; j++) {
//				vector<int*> newpath;
//				for (int i = 0; i < cutpath.size(); i++) {
//					newpath.push_back(cutpath[i]);
//				}
//				temppathlist.push_back(newpath);
//			}
//		nextpoint:
//			while (tolookpoint[tolookpoint.size() - 1].second.size() > 0) {
//				tempx = tolookpoint[tolookpoint.size() - 1].second[tolookpoint[tolookpoint.size() - 1].second.size() - 1][0];
//				tempy = tolookpoint[tolookpoint.size() - 1].second[tolookpoint[tolookpoint.size() - 1].second.size() - 1][1];
//				int* a = new int[2];
//				a[0] = tempx;
//				a[1] = tempy;
//				cutpath.push_back(a);
//				passed_allpoints.push_back(a);
//				if (tolookpoint[tolookpoint.size() - 1].second.size() < (temp_all_num[temp_all_num.size() - 1])) {
//					temppathlist.pop_back();
//				}
//				tolookpoint[tolookpoint.size() - 1].second.pop_back();
//
//				Mat tt = tc_up.clone();
//				for (int m = 0; m < cutpath.size(); m++) {
//					tt.at<uchar>(cutpath[m][0], cutpath[m][1]) = 255;
//				}
//				imshow("fc", tt);
//				waitKey();
//				goto lookdown;
//			}
//			tolookpoint.pop_back();
//			temp_all_num.pop_back();
//			goto nextpoint;
//		}
//		filter_neighbor.clear();
//	}
//
//	if (cutpath.size() > 0) {
//		resultpath.push_back(cutpath);
//	}
//	imshow("1", dst);
//	if (tolookpoint[tolookpoint.size() - 1].second.size() > 0) {
//		Mat temppic = tc_up.clone();
//		for (int j = 0; j < cutpath.size(); j++) {
//			temppic.at<uchar>(cutpath[j][0], cutpath[j][1]) = 255;
//		}
//		imshow("2", temppic);
//		waitKey();
//		cutpath.clear();
//		cutpath = temppathlist[temppathlist.size() - 1];
//		goto nextpoint;
//	}
//	else {
//		if ((tolookpoint.size() - 1) > 0) {
//			tolookpoint.pop_back();
//			temp_all_num.pop_back();
//			cutpath.clear();
//			if (temppathlist.size() > 0) {
//				cutpath = temppathlist[temppathlist.size() - 1];
//				goto nextpoint;
//			}
//		}
//	}
//}
//
//int main() {
//	Mat src = imread("C:\\Users\\Administrator\\Desktop\\123\\91.png");	
//	cvtColor(src, src, CV_BGR2GRAY);
//	Mat srcclone = src.clone();
//
//	Mat tempsrc = src.clone();					//去掉圈内背景的背景骨架
//	threshold(tempsrc, tempsrc, 0, 255, CV_THRESH_OTSU);
//	copyMakeBorder(tempsrc, tempsrc, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));
//	Mat tc_up = tempsrc.clone();
//	Mat tc_down = tempsrc.clone();
//	Mat tc_result = tempsrc.clone();
//	flip(tc_down, tc_down, 0);
//	flip(tc_down, tc_down, 1);
//	IplImage *img = &IplImage(tempsrc);
//	CvMemStorage * storage = cvCreateMemStorage(0);
//	CvSeq *contours = 0;
//	cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//	cvDrawContours(img, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);
//	Mat s1(img);
//	bitwise_not(s1, s1);
//	cv::threshold(s1, s1, 128, 1, cv::THRESH_BINARY);
//	Mat dst = thinImage(s1, -1);
//	Mat dstclone_for_crosspoint = dst.clone();
//	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 0);
//	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 1);
//	dst = dst * 255;
//	dstclone_for_crosspoint = dstclone_for_crosspoint * 255;
//
//	vector<vector<int*>> resultpath_up;
//	vector<vector<int*>> resultpath_down;
//	getAll_cutpath(resultpath_up, dst, tc_up, true);
//	getAll_cutpath(resultpath_down, dstclone_for_crosspoint, tc_down, false);
//
//	for (int i = 0; i < resultpath_down.size(); i++) {
//		//for (int j = 0; j < resultpath_down[i].size(); j++) {
//		resultpath_down[i][resultpath_down[i].size() - 1][0] = dst.rows - 1 - resultpath_down[i][resultpath_down[i].size() - 1][0];
//		resultpath_down[i][resultpath_down[i].size() - 1][1] = dst.cols - 1 - resultpath_down[i][resultpath_down[i].size() - 1][1];
//		//}
//	}
//	int result_uppath_id = 0;
//	int result_downpath_id = 0;
//	int mindistance = 9999999;
//	for (int i = 0; i < resultpath_up.size(); i++) {
//		for (int j = 0; j < resultpath_down.size(); j++) {
//			int distance = pow(resultpath_up[i][resultpath_up[i].size() - 1][0] - resultpath_down[j][resultpath_down[j].size() - 1][0], 2)
//				+ pow(resultpath_up[i][resultpath_up[i].size() - 1][1] - resultpath_down[j][resultpath_down[j].size() - 1][1], 2);
//			if (distance < mindistance) {
//				mindistance = distance;
//				result_uppath_id = i;
//				result_downpath_id = j;
//			}
//		}
//	}
//	for (int j = 0; j < resultpath_down[result_downpath_id].size() - 1; j++) {
//		resultpath_down[result_downpath_id][j][0] = dst.rows - 1 - resultpath_down[result_downpath_id][j][0];
//		resultpath_down[result_downpath_id][j][1] = dst.cols - 1 - resultpath_down[result_downpath_id][j][1];
//	}
//	for (int i = 0; i < resultpath_up[result_uppath_id].size(); i++) {
//		tc_result.at<uchar>(resultpath_up[result_uppath_id][i][0], resultpath_up[result_uppath_id][i][1]) = 255;
//	}
//	imshow("vfdvgb2", tc_result);
//	waitKey();
//	for (int i = 0; i < resultpath_down[result_downpath_id].size(); i++) {
//		tc_result.at<uchar>(resultpath_down[result_downpath_id][i][0], resultpath_down[result_downpath_id][i][1]) = 255;
//	}
//	imshow("vfdvgb3", tc_result);
//	waitKey();
//	line(tc_result, Point(resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][1],
//		resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][0]),
//		Point(resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][1],
//			resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][0]), Scalar(255));
//	imshow("vfdvgb4", tc_result);
//	waitKey();
//	/*line(tc_result, Point(resultpath_up[result_uppath_id][0][1], 0), Point(resultpath_up[result_uppath_id][0][1], resultpath_up[result_uppath_id][0][0]), Scalar(255));
//	imshow("vfdvgb5", tc_result);
//	waitKey();
//	line(tc_result, Point(resultpath_down[result_downpath_id][0][1], tc_result.rows - 1), Point(resultpath_down[result_downpath_id][0][1], resultpath_down[result_downpath_id][0][0]), Scalar(255));
//	imshow("vfdvgb", tc_result);
//	waitKey();*/
//	return 0;
//}










void getAll_cutpath(vector<vector<int*>>& resultpath, Mat dst, Mat tc_up, bool up) {
	vector<int*> backcoord;
	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++) {
			if ((int)dst.at<uchar>(i, j) == 255) {
				int* a = new int[2];
				a[0] = i;
				a[1] = j;
				backcoord.push_back(a);
			}
		}
	sort(backcoord.begin(), backcoord.end(), contoursizeCmp);
	int left = backcoord[0][1] * 0.85 + 0.15 * backcoord[backcoord.size() - 1][1];
	int right = backcoord[backcoord.size() - 1][1] * 0.85 + 0.15 * backcoord[0][1];
	int lefth = gethighestpoint_x(backcoord, left, right)[0];
	int righth = gethighestpoint_x(backcoord, left, right)[1];
	int tempx = lefth;
	int tempy = left;
	line(dst, Point(left - 2, 0), Point(left - 2, dst.rows - 1), Scalar(0));
	line(dst, Point(right + 2, 0), Point(right + 2, dst.rows - 1), Scalar(0));
	/*line(dst, Point(left - 2, 0), Point(left - 2, lefth + 5), Scalar(0));
	line(dst, Point(right + 2, 0), Point(right + 2, righth + 5), Scalar(0));
	dst.at<uchar>(lefth + 5, left - 2) = 255;
	dst.at<uchar>(righth + 5, right + 2) = 255;*/

	vector<vector<int*>> temppathlist;
	vector<int*> temp;
	int* next;
	vector<pair<int*, vector<int*>>> tolookpoint;
	vector<int*> cutpath;
	vector<int*> passed_allpoints;
	vector<int> temp_all_num;
	int* crosspoint = new int[2];
	int* a = new int[2];
	a[0] = tempx;
	a[1] = tempy;
	cutpath.push_back(a);
	passed_allpoints.push_back(a);
	vector<int*> neighbor = getneighbor(dst, tempx, tempy);
	vector<int*> filter_neighbor;
	while (neighbor.size() != 1) {
		for (int n = 0; n < neighbor.size(); n++) {
			if (!(existin_passed_points(neighbor[n], passed_allpoints))) {
				filter_neighbor.push_back(neighbor[n]);
			}
		}
		if (filter_neighbor.size() <= 1) {
			if (filter_neighbor.size() == 1) {
				tempx = filter_neighbor[0][0];
				tempy = filter_neighbor[0][1];
				int* a = new int[2];
				a[0] = tempx;
				a[1] = tempy;
				cutpath.push_back(a);
				passed_allpoints.push_back(a);
			}
			else {
				break;
			}
		lookdown:
			neighbor = getneighbor(dst, tempx, tempy);

		}
		else if (filter_neighbor.size() > 1) {
			crosspoint[0] = tempx;
			crosspoint[1] = tempy;
			tolookpoint.push_back(make_pair(crosspoint, filter_neighbor));
			temp_all_num.push_back(filter_neighbor.size());
			for (int j = 0; j < filter_neighbor.size() - 1; j++) {
				vector<int*> newpath;
				for (int i = 0; i < cutpath.size(); i++) {
					newpath.push_back(cutpath[i]);
				}
				temppathlist.push_back(newpath);
			}
		nextpoint:
			while (tolookpoint[tolookpoint.size() - 1].second.size() > 0) {
				tempx = tolookpoint[tolookpoint.size() - 1].second[tolookpoint[tolookpoint.size() - 1].second.size() - 1][0];
				tempy = tolookpoint[tolookpoint.size() - 1].second[tolookpoint[tolookpoint.size() - 1].second.size() - 1][1];
				int* a = new int[2];
				a[0] = tempx;
				a[1] = tempy;
				cutpath.push_back(a);
				passed_allpoints.push_back(a);
				if (tolookpoint[tolookpoint.size() - 1].second.size() < (temp_all_num[temp_all_num.size() - 1])) {
					temppathlist.pop_back();
				}
				tolookpoint[tolookpoint.size() - 1].second.pop_back();

				Mat tt = tc_up.clone();
				for (int m = 0; m < cutpath.size(); m++) {
					tt.at<uchar>(cutpath[m][0], cutpath[m][1]) = 255;
				}
				imshow("fc", tt);
				waitKey();
				goto lookdown;
			}
			tolookpoint.pop_back();
			temp_all_num.pop_back();
			goto nextpoint;
		}
		filter_neighbor.clear();
	}

	if (cutpath.size() > 0) {
		resultpath.push_back(cutpath);
	}
	imshow("1", dst);
	if (tolookpoint[tolookpoint.size() - 1].second.size() > 0) {
		Mat temppic = tc_up.clone();
		for (int j = 0; j < cutpath.size(); j++) {
			temppic.at<uchar>(cutpath[j][0], cutpath[j][1]) = 255;
		}
		imshow("2", temppic);
		waitKey();
		cutpath.clear();
		cutpath = temppathlist[temppathlist.size() - 1];
		goto nextpoint;
	}
	else {
		if ((tolookpoint.size() - 1) > 0) {
			tolookpoint.pop_back();
			temp_all_num.pop_back();
			cutpath.clear();
			if (temppathlist.size() > 0) {
				cutpath = temppathlist[temppathlist.size() - 1];
				goto nextpoint;
			}
		}
	}
}

vector<Mat> getpics_with_cutpath(Mat src, Mat dst, int up_left, int down_right, Point up_last, Point down_last) {
	vector<Mat> result;
	Mat skeleton = dst - src;
	line(skeleton, Point(0, 0), Point(up_left, 0), Scalar(255));
	line(skeleton, Point(0, 0), Point(0, skeleton.rows - 1), Scalar(255));
	line(skeleton, Point(0, skeleton.rows - 1), Point(down_right, skeleton.rows - 1), Scalar(255));
	line(skeleton, up_last, down_last, Scalar(255));
	imshow("ske", skeleton);
	IplImage *img = &IplImage(skeleton);
	cvFloodFill(img, cvPoint(1, 1), cvScalarAll(255));
	Mat mask(img);
	imshow("mask", mask);
	Mat result1 = src.clone();
	Mat result2 = src.clone();
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if ((int)mask.at<uchar>(i, j) == 0) {
				result1.at<uchar>(i, j) = 0;
			}
		}
	}
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if ((int)mask.at<uchar>(i, j) == 255) {
				result2.at<uchar>(i, j) = 0;
			}
		}
	}
	imshow("result1", result1);
	imshow("result2", result2);
	result.push_back(result1);
	result.push_back(result2);
	waitKey();
	return result;
}

int main() {
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\123\\109.png");
	//Mat src = imread("C:\\Users\\Administrator\\Desktop\\wrong\\137.png");
	cvtColor(src, src, CV_BGR2GRAY);
	Mat srcclone = src.clone();

	Mat tempsrc = src.clone();					//去掉圈内背景的背景骨架
	threshold(tempsrc, tempsrc, 0, 255, CV_THRESH_OTSU);
	//bitwise_not(tempsrc, tempsrc);
	copyMakeBorder(tempsrc, tempsrc, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));
	Mat tc_up = tempsrc.clone();
	Mat tc_down = tempsrc.clone();
	Mat tc_result = tempsrc.clone();
	Mat tocut = tempsrc.clone();
	flip(tc_down, tc_down, 0);
	flip(tc_down, tc_down, 1);
	IplImage *img = &IplImage(tempsrc);
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	cvFindContours(img, storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	cvDrawContours(img, contours, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), -1, CV_FILLED, 8);
	Mat s1(img);
	bitwise_not(s1, s1);
	cv::threshold(s1, s1, 128, 1, cv::THRESH_BINARY);
	Mat dst = thinImage(s1, -1);
	Mat dstclone_for_crosspoint = dst.clone();
	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 0);
	flip(dstclone_for_crosspoint, dstclone_for_crosspoint, 1);
	dst = dst * 255;
	//imshow("vj", dst);
	//waitKey();
	dstclone_for_crosspoint = dstclone_for_crosspoint * 255;

	vector<vector<int*>> resultpath_up;
	vector<vector<int*>> resultpath_down;
	getAll_cutpath(resultpath_up, dst, tc_up, true);
	getAll_cutpath(resultpath_down, dstclone_for_crosspoint, tc_down, false);

	for (int i = 0; i < resultpath_down.size(); i++) {
		//for (int j = 0; j < resultpath_down[i].size(); j++) {
		resultpath_down[i][resultpath_down[i].size() - 1][0] = dst.rows - 1 - resultpath_down[i][resultpath_down[i].size() - 1][0];
		resultpath_down[i][resultpath_down[i].size() - 1][1] = dst.cols - 1 - resultpath_down[i][resultpath_down[i].size() - 1][1];
		//}
	}
	int result_uppath_id = 0;
	int result_downpath_id = 0;
	int mindistance = 9999999;
	for (int i = 0; i < resultpath_up.size(); i++) {
		for (int j = 0; j < resultpath_down.size(); j++) {
			int distance = pow(resultpath_up[i][resultpath_up[i].size() - 1][0] - resultpath_down[j][resultpath_down[j].size() - 1][0], 2)
				+ pow(resultpath_up[i][resultpath_up[i].size() - 1][1] - resultpath_down[j][resultpath_down[j].size() - 1][1], 2);
			if (distance < mindistance) {
				mindistance = distance;
				result_uppath_id = i;
				result_downpath_id = j;
			}
		}
	}
	for (int j = 0; j < resultpath_down[result_downpath_id].size() - 1; j++) {
		resultpath_down[result_downpath_id][j][0] = dst.rows - 1 - resultpath_down[result_downpath_id][j][0];
		resultpath_down[result_downpath_id][j][1] = dst.cols - 1 - resultpath_down[result_downpath_id][j][1];
	}
	for (int i = 0; i < resultpath_up[result_uppath_id].size(); i++) {
		tc_result.at<uchar>(resultpath_up[result_uppath_id][i][0], resultpath_up[result_uppath_id][i][1]) = 255;
	}
	imshow("vfdvgb2", tc_result);
	//waitKey();
	for (int i = 0; i < resultpath_down[result_downpath_id].size(); i++) {
		tc_result.at<uchar>(resultpath_down[result_downpath_id][i][0], resultpath_down[result_downpath_id][i][1]) = 255;
	}
	imshow("vfdvgb3", tc_result);
	//waitKey();
	line(tc_result, Point(resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][1],
		resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][0]),
		Point(resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][1],
			resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][0]), Scalar(255));
	imshow("vfdvgb4", tc_result);
	//waitKey();
	line(tc_result, Point(resultpath_up[result_uppath_id][0][1], 0), Point(resultpath_up[result_uppath_id][0][1], resultpath_up[result_uppath_id][0][0]), Scalar(255));
	imshow("vfdvgb5", tc_result);
	//waitKey();
	line(tc_result, Point(resultpath_down[result_downpath_id][0][1], tc_result.rows - 1), Point(resultpath_down[result_downpath_id][0][1], resultpath_down[result_downpath_id][0][0]), Scalar(255));
	imshow("vfdvgb", tc_result);
	waitKey();

	vector<Mat> split_imgs = getpics_with_cutpath(tocut, tc_result, resultpath_up[result_uppath_id][0][1], resultpath_down[result_downpath_id][0][1], 
		Point(resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][1],
		resultpath_up[result_uppath_id][resultpath_up[result_uppath_id].size() - 1][0]),
		Point(resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][1],
			resultpath_down[result_downpath_id][resultpath_down[result_downpath_id].size() - 1][0]));
	return 0;
}









bool compareVec4ix(Vec4i a, Vec4i b) {
	return a[0] < b[0];
}

bool compareVec4iy(Vec4i a, Vec4i b) {
	return a[1] < b[1];
}
bool file_string_cmp(string str1, string str2) {
	int pos1l = str1.find_last_of('/');
	int pos1r = str1.find_last_of('.');
	int pos2l = str2.find_last_of('/');
	int pos2r = str2.find_last_of('.');
	string n1(str1.substr(pos1l + 1, pos1r - pos1l));
	string n2(str2.substr(pos2l + 1, pos2r - pos2l));
	return atoi(n1.c_str()) < atoi(n2.c_str());
}
int main_vdgnhgm() {
	string dir = "C:\\Users\\Administrator\\Desktop\\image";
	vector<string> files;
	getAllFiles(dir, files);
	sort(files.begin(), files.end(), file_string_cmp);
	for (int f_num = 0; f_num < files.size(); f_num++) {
		//Mat src = imread("C:\\Users\\Administrator\\Desktop\\image\\327.png");
		Mat src = imread(files[f_num]);
		int pos = files[f_num].find_last_of('\\');
		string name(files[f_num].substr(pos + 1));
		Mat src1 = src.clone();
		Mat cut = src.clone();
		vector<Vec4i> Lines;
		vector<Vec4i> Lines1;
		Mat CannyImg;
		Canny(src, CannyImg, 140, 250, 3);
		//imshow("vfv", CannyImg);
		//waitKey();
		HoughLinesP(CannyImg, Lines, 1, CV_PI / 180, 10, 10, 1);
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


		HoughLinesP(CannyImg, Lines1, 1, CV_PI / 180, 30, 30, 1);
		vector<Vec4i>::const_iterator itc1 = Lines1.begin();
		while (itc1 != Lines1.end())
		{
			if (abs((*itc1)[1] - (*itc1)[3]) > 0)
				itc1 = Lines1.erase(itc1);
			else
			{
				++itc1;
			}
		}

		for (int i = 0; i < Lines.size(); i++)
		{
			line(src1, Point(Lines[i][0], Lines[i][1]), Point(Lines[i][2], Lines[i][3]), Scalar(255, 0, 0), 1, 8);
		}
		/*imshow("nn", src1);
		waitKey();*/
		sort(Lines.begin(), Lines.end(), compareVec4ix);
		sort(Lines1.begin(), Lines1.end(), compareVec4iy);
		vector<int*> newLines;
		vector<int> newLines1;
		if (Lines.size() != 0) {
			for (int i = 1; i < Lines.size(); i++) {
				int pre = Lines[i - 1][0];
				int cur = Lines[i][0];
				if ((cur - pre) < 18) {
					continue;
				}
				else {
					int* a = new int[2];
					a[0] = pre;
					a[1] = cur;
					newLines.push_back(a);
				}
			}
		}

		if (newLines.size() > 0) {
			int* fir = new int[2];
			fir[0] = 0;
			fir[1] = newLines[0][0];
			newLines.insert(newLines.begin(), fir);
			int* end = new int[2];
			end[0] = newLines[newLines.size() - 1][1];
			end[1] = src.cols;
			newLines.push_back(end);
		}
		if (Lines1.size() != 0) {
			if (Lines1[Lines1.size() - 1][1] < src.rows / 2) {
				newLines1.push_back(Lines1[Lines1.size() - 1][1]);
			}
			else {
				for (int i = 0; i < Lines1.size() - 1; i++) {
					if ((Lines1[i][1] < src.rows / 2) && (Lines1[i + 1][1] > src.rows / 2)) {
						newLines1.push_back(Lines1[i][1]);
						newLines1.push_back(Lines1[i + 1][1]);
						break;
					}
				}
				if (newLines1.size() == 0) {
					for (int i = 0; i < Lines1.size() - 1; i++) {
						if (Lines1[i][1] > src.rows / 2) {
							newLines1.push_back(Lines1[i][1]);
							break;
						}
					}
				}

			}
		}



		for (int i = 0; i < newLines.size(); i++)
		{
			line(src, Point(newLines[i][0], 0), Point(newLines[i][0], src.rows - 1), Scalar(255, 0, 0), 1, 8);
			line(src, Point(newLines[i][1], 0), Point(newLines[i][1], src.rows - 1), Scalar(255, 0, 0), 1, 8);
		}

		for (int i = 0; i < newLines1.size(); i++)
		{
			line(src, Point(0, newLines1[i]), Point(src.cols - 1, newLines1[i]), Scalar(255, 0, 0), 1, 8);
		}
		/*imshow("nn", src);
		waitKey();*/

		char save_file[300];
		string s = "C:\\Users\\Administrator\\Desktop\\tempcut";
		int num = 0;
		string s2 = s + "\\" + name + "_%d.png";
		for (int i = 0; i < newLines.size(); i++) {
			int up = 0;
			int down = 0;
			if (newLines1.size() == 0) {
				up = 0;
				down = src.rows;
			}
			else if (newLines1.size() == 1) {
				if (newLines1[0] > src.rows / 2) {
					up = 0;
					down = newLines1[0];
				}
				else {
					up = newLines1[0];
					down = src.rows;
				}
			}
			else {
				up = newLines1[0];
				down = newLines1[1];
			}
			if ((newLines[i][1] - newLines[i][0]) > 2) {
				Mat temp = cut(Range(up + 3, down), Range(newLines[i][0] + 1, newLines[i][1] - 1));
				if (temp.cols > 15) {
					if (temp.cols < 25) {
						sprintf(save_file, s2.c_str(), num);
						imwrite(save_file, temp);
						num++;
					}
					else {
						Mat temp1 = temp(Range(0, temp.rows), Range(0, temp.cols / 2));
						Mat temp2 = temp(Range(0, temp.rows), Range(temp.cols / 2, temp.cols));
						if (temp1.cols > 15) {
							sprintf(save_file, s2.c_str(), num);
							imwrite(save_file, temp1);
							num++;
						}
						if (temp2.cols > 15) {
							sprintf(save_file, s2.c_str(), num);
							imwrite(save_file, temp2);
							num++;
						}
					}
				}
			}

		}


	}
	return 0;
}

