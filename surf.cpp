#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include <io.h>
using namespace std;
using namespace cv;
//int main(int argc, char** argv)
//{
//	Mat img_1 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\before.jpg", 0);    
//	Mat img_2 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\after.jpg", 0); 
//	int minHessian = 10000;    
//	SiftFeatureDetector detector(minHessian);    
//	std::vector<KeyPoint> keypoints_1, keypoints_2;    
//	detector.detect(img_1, keypoints_1);    
//	detector.detect(img_2, keypoints_2);
//	Mat img_keypoints_1; Mat img_keypoints_2;    
//	drawKeypoints(img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);    
//	drawKeypoints(img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//	SurfDescriptorExtractor extractor;    
//	Mat descriptors_1, descriptors_2;    
//	extractor.compute(img_1, keypoints_1, descriptors_1);    
//	extractor.compute(img_2, keypoints_2, descriptors_2);
//	BFMatcher matcher(NORM_L2);
//	std::vector< DMatch > matches;
//	matcher.match(descriptors_1, descriptors_2, matches);
//	/*Mat img_matches;
//	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);
//	imshow("Keypoints 1", img_keypoints_1);
//	imshow("Keypoints 2", img_keypoints_2);
//	imshow("Matches", img_matches);*/
//	double max_dist = 0; double min_dist = 100;
//	for (int i = 0; i < descriptors_1.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist)
//			min_dist = dist;
//		if (dist > max_dist)
//			max_dist = dist;
//	}
//	printf("-- Max dist : %f \n", max_dist);
//	printf("-- Min dist : %f \n", min_dist);
//	std::vector< DMatch > good_matches;
//	for (int i = 0; i < descriptors_1.rows; i++) 
//	{ 
//		if (matches[i].distance <= max(2 * min_dist, 0.02)) 
//		{ 
//			good_matches.push_back(matches[i]); 
//		} 
//	}
//	Mat img_matches;    
//	drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//	imshow("Good Matches", img_matches); 
//	/*for (int i = 0; i < (int)good_matches.size(); i++) 
//	{ 
//		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx); 
//	}*/
//	std::vector<Point2f> obj;    
//	std::vector<Point2f> scene; 
//	for (int i = 0; i < (int)good_matches.size(); i++) 
//	{ 
//		obj.push_back(keypoints_1[good_matches[i].queryIdx].pt);        
//		scene.push_back(keypoints_2[good_matches[i].trainIdx].pt);        
//		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx); 
//	}
//	Mat H = findHomography(obj, scene, CV_RANSAC);     
//	std::vector<Point2f> obj_corners(4);    
//	obj_corners[0] = Point(0,0); 
//	obj_corners[1] = Point( img_1.cols, 0 );    
//	obj_corners[2] = Point( img_1.cols, img_1.rows ); 
//	obj_corners[3] = Point( 0, img_1.rows );    
//	std::vector<Point2f> scene_corners(4);    
//	perspectiveTransform( obj_corners, scene_corners, H);
//	Point2f offset((float)img_1.cols, 0);    
//	line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);    
//	line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);    
//	line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);    
//	line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);
//	imshow("Good Matches & Object detection", img_matches);
//	waitKey(0);
//	return 0;
//}


int main_surf(int argc, char* argv[])
{
	Mat img_1 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\before.jpg", CV_LOAD_IMAGE_GRAYSCALE);//宏定义时CV_LOAD_IMAGE_GRAYSCALE=0，也就是读取灰度图像
	Mat img_2 = imread("C:\\Users\\Administrator\\Desktop\\转账前后\\after.jpg", CV_LOAD_IMAGE_GRAYSCALE);//一定要记得这里路径的斜线方向，这与Matlab里面是相反的

	if (!img_1.data || !img_2.data)//如果数据为空
	{
		cout << "opencv error" << endl;
		return -1;
	}
	cout << "open right" << endl;

	//第一步，用SURF算子检测关键点
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);
	std::vector<KeyPoint> keypoints_1, keypoints_2;//构造2个专门由点组成的点向量用来存储特征点

	detector.detect(img_1, keypoints_1);//将img_1图像中检测到的特征点存储起来放在keypoints_1中
	detector.detect(img_2, keypoints_2);//同理

										//在图像中画出特征点
	Mat img_keypoints_1, img_keypoints_2;

	drawKeypoints(img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	imshow("surf_keypoints_1", img_keypoints_1);
	imshow("surf_keypoints_2", img_keypoints_2);

	//计算特征向量
	SurfDescriptorExtractor extractor;//定义描述子对象

	Mat descriptors_1, descriptors_2;//存放特征向量的矩阵

	extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(img_2, keypoints_2, descriptors_2);

	//用burte force进行匹配特征向量
	BFMatcher matcher(NORM_L2);//定义一个burte force matcher对象
	vector<DMatch>matches;
	matcher.match(descriptors_1, descriptors_2, matches);
	cout << "" << endl;
	//绘制匹配线段
	Mat img_matches;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);//将匹配出来的结果放入内存img_matches中

																			  //显示匹配线段
	imshow("surf_Matches", img_matches);//显示的标题为Matches
	waitKey(0);
	return 0;
}
