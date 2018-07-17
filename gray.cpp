#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>  
#include "lib/normal.h"  
#include "lib/cutchar.h"  

#define DEBUG  

#ifdef DEBUG  
#define DE(format, ...) printf(format, ## __VA_ARGS__)  
#else  
#define DE(format, ...) while(0)  
#endif  

//cutchar.h-begin
#ifndef _CUTCHAR_H__
#define __CUTCHAR_H__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <string.h>
#include <opencv/cv.h>
#include <stdio.h>

using namespace cv;

/*返回二值化图像mat1，满足至少发生number次跳变的开始行数*/
int detectionChange(Mat& mat1, Mat& mat2, int number);

/*将图片mat1 归一化到mat2( width,height大小)*/
void carCard_Resize(Mat& mat1, Mat& mat2, int width, int height);

/*垂直投影计算*/
void verProjection_calculate(Mat& mat1, int* vArr, int number);

/*根据投影分割图片*/
int** verProjection_cut(int* vArr, int width, int* number);

/*检测传入图像是否是1*/
float pixelPercentage(Mat& mat1);

#endif

//cutchar.h-end


//cutchar.cpp-begin

void carCard_Resize(Mat& mat1, Mat& mat2, int width, int height) {
	IplImage pI_1 = mat1, pI_2;

	mat2 = cv::Mat(width, height, CV_8UC1, 1);
	pI_2 = mat2;

	cvResize(&pI_1, &pI_2, 1);
}

int detectionChange(Mat& mat1, Mat& mat2, int number) {
	IplImage pI_1 = mat1, pI_2;
	CvScalar s1, s2;
	int width = mat1.rows;
	int height = mat1.cols;
	int sum = 0, sum_2 = 0, width_1 = 0, width_2 = 0;
	int i, j;

	for (i = 0; i<width; i++) {
		sum = 0;
		sum_2 = 0;
		for (j = 0; j<height - 1; j++) {
			s1 = cvGet2D(&pI_1, i, j);
			s2 = cvGet2D(&pI_1, i, j + 1);
			if (((int)s1.val[0]) != ((int)s2.val[0])) {
				sum += 1;
				sum_2 = 0;
			}
			else {
				sum_2 += 1;
			}
			if (sum_2 != 0) {
				if (height / sum_2 < 5) {
					sum = 0;
					break;
				}
			}
		}
		if (sum >= number) {
			width_1 = i;
			break;
		}
		else {
			width_1 = i;
		}
	}

	for (i = width - 1; i> 0; i--) {
		sum = 0;
		sum_2 = 0;
		for (j = 0; j<height - 1; j++) {
			s1 = cvGet2D(&pI_1, i, j);
			s2 = cvGet2D(&pI_1, i, j + 1);
			if (((int)s1.val[0]) != ((int)s2.val[0])) {
				sum += 1;
				sum_2 = 0;
			}
			else {
				sum_2 += 1;
			}
			if (sum_2 != 0) {
				if (height / sum_2 < 1) {
					sum = 0;
					break;
				}
			}
		}
		if (sum >= number) {
			width_2 = i;
			break;
		}
		else {
			width_2 = i;
		}
	}
	if (width_2 <= width_1) {
		width_2 = width;
	}
	mat2 = cv::Mat(width_2 - width_1 + 1, height, CV_8UC1, 1);
	pI_2 = mat2;
	for (i = width_1; i <= width_2; i++) {
		for (j = 0; j<height; j++) {
			s1 = cvGet2D(&pI_1, i, j);
			cvSet2D(&pI_2, i - width_1, j, s1);
		}
	}
	return 0;
}

void verProjection_calculate(Mat& mat1, int* vArr, int number) {
	IplImage pI_1 = mat1;
	CvScalar s1;
	int width = mat1.rows;
	int height = mat1.cols;
	int i, j;

	for (i = 0; i< number; i++) {
		vArr[i] = 0;
	}

	for (j = 0; j<height; j++) {
		for (i = 0; i<width; i++) {
			s1 = cvGet2D(&pI_1, i, j);
			if (s1.val[0] > 20) {
				vArr[j] += 1;
			}
		}
	}
}

int** verProjection_cut(int* vArr, int width, int* number) {
	int **a;
	int i, flag = 0;
	int num = 0;
	int threshold = 2;

	a = (int**)malloc(width / 2 * sizeof(int*));

	for (i = 0; i<width - 1; i++) {
		if ((vArr[i] <= threshold) && (vArr[i + 1] > threshold)) {
			a[num] = (int*)malloc(2 * sizeof(int));
			a[num][0] = i;
			//修改num值
			//		num += 1;
			flag = 1;
		}
		else if ((vArr[i] > threshold) && (vArr[i + 1] <= threshold) && (flag != 0)) {
			a[num][1] = i;
			num += 1;
			flag = 0;
		}
	}
	*number = num;

	return a;
}

float pixelPercentage(Mat& mat1) {
	IplImage pI_1 = mat1;
	CvScalar s1;
	int width = mat1.rows;
	int height = mat1.cols;
	int i, j;
	float sum = 0, allSum = 0, tmp;

	for (i = 0; i<width; i++) {
		for (j = 0; j<height; j++) {
			s1 = cvGet2D(&pI_1, i, j);
			if (s1.val[0] > 20) {
				sum += 1;
			}
			allSum += 1;
		}
	}
	tmp = sum / allSum;

	return tmp;
}


//cutchar.cpp-end
//normal.h-begin
#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <string.h>
#include <opencv/cv.h>
#include <stdio.h>

using namespace cv;

/*将string转化为double*/
double char2num(char *s);

/*图像灰度化*/
void pic_gray(Mat& mat1, Mat& mat2);

/*图像二值化*/
void pic_Thresholding(Mat& mat1, int threshold);

/*Soble边沿检测*/
void pic_Sobel(Mat& mat1, double x_beta);

/*直方图计算*/
int histogram_Calculate(Mat& mat1, int number);

/*筛选步骤1*/
int** selection_Function_1(Mat& mat1, int* number);

/*返回第一步裁减分割后图片*/
void pic_cutting(Mat& mat1, Mat& pic_cutting, int** selection, int number);

/*裁减出一张图片*/
void pic_cutting_1(Mat& mat1, Mat& mat2, Point s1, Point s2);

/*通过HSV颜色空间来检测该图片是否是车牌所在图片*/
int choice_Color(Mat& mat1, int color_Start, int color_End);

/*在检测出车牌的图片中，框选车牌*/
int** car_License_box(Mat& mat1, Mat& mat2, int* number);

/*在框选出的方框中筛选出可能是车牌的方框*/
int box_selection(Mat& mat1);

#endif


//normal.h-end

//normal.cpp-begin
double char2num(char *s) {
	double d = 0;
	int i, t = 0;

	for (i = 0; s[i] != 0; i++) {
		if (s[i] == '.') {
			t = 10;
		}
		else {
			if (t == 0) {
				d = d * 10 + (s[i] - '0');
			}
			else {
				d = d + (double)(s[i] - '0') / t;
				t *= 10;
			}
		}
	}
	return d;
}

void pic_gray(Mat& mat1, Mat& mat2) {
	IplImage pI = mat1;
	uchar* ptr;
	CvScalar s;

	int width = mat1.rows;
	int height = mat1.cols;

	mat2 = cv::Mat(width, height, CV_8UC1, 1);
	ptr = mat2.ptr(0);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j<height; j++) {
			s = cvGet2D(&pI, i, j);
			int grayScale = (int)(s.val[0] * 0.299 + s.val[1] * 0.587 + s.val[2] * 0.114);
			ptr[i*height + j] = grayScale;
		}
	}
}

void pic_Thresholding(Mat& mat1, int threshold) {
	uchar* ptr = mat1.ptr(0);
	int width = mat1.rows;
	int height = mat1.cols;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j<height; j++) {
			if (ptr[i*height + j] > 125) {
				ptr[i*height + j] = 255;
			}
			else {
				ptr[i*height + j] = 0;
			}
		}
	}
}

void pic_Sobel(Mat& mat1, double x_beta) {
	Mat dst_x, dst_y;

	Sobel(mat1, dst_x, mat1.depth(), 1, 0);
	Sobel(mat1, dst_y, mat1.depth(), 0, 1);
	convertScaleAbs(dst_x, dst_x);
	convertScaleAbs(dst_y, dst_y);
	addWeighted(dst_x, x_beta, dst_y, (1 - x_beta), 0, mat1);
}

int histogram_Calculate(Mat& mat1, int number) {
	Mat gray_hist;
	int histSize = 255;
	float range[] = { 0, 255 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	int width, height;
	int i, j;
	uchar* ptr = mat1.ptr(0);
	long int pixel_all = 0, pixel_Calc = 0;

	calcHist(&mat1, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate);

	width = gray_hist.rows;
	height = gray_hist.cols;

	for (i = 0; i <= width; i++) {
		pixel_all += ptr[i];
	}

	for (i = 0; i <= width; i++) {
		pixel_Calc += ptr[255 - i];
		if (((pixel_Calc * 100) / pixel_all) > number) {
			i = 255 - i;
			break;
		}
	}
	return i;
}
int** selection_Function_1(Mat& mat1, int* number) {
	int **a, i, j, flag, num = 0, enter_flag = 0;
	int width = mat1.rows;
	int height = mat1.cols;
	uchar* ptr = mat1.ptr(0);

	a = (int**)malloc(width * sizeof(int*));

	for (i = 0; i<width; i++) {
		flag = 0;
		for (j = 0; j< height - 1; j++) {
			if (ptr[i*height + j] != ptr[i*height + j + 1]) {
				flag += 1;
			}
		}
		if ((flag >= 7) && (enter_flag == 0)) {
			a[num] = (int*)malloc(2 * sizeof(int));
			a[num][0] = i;
			enter_flag = 1;
		}
		else if ((enter_flag != 0) && (flag < 7)) {
			if (i - a[num][0] < 8) {
				continue;
			}
			a[num][1] = i - 1;
			num++;
			enter_flag = 0;
		}
	}
	*number = num;
	return a;
}

int choice_Color(Mat& mat1, int color_Start, int color_End) {
	int width = mat1.rows;
	int height = mat1.cols;
	uchar* ptr = mat1.ptr(0);
	IplImage pI_1;
	int *flag = new int[width];

	int num, i, j, num_width = 0;
	CvScalar s;

	pI_1 = mat1;
	cvCvtColor(&pI_1, &pI_1, CV_BGR2HSV);
	for (i = 0; i<width; i++) {
		num = 0;
		for (j = 0; j<height; j++) {
			s = cvGet2D(&pI_1, i, j);
			if ((s.val[0] >= color_Start) && (s.val[0] <= color_End)) {
				num += 1;
			}
		}
		if (num > 20) {
			flag[i] = 1;
			num_width += 1;
		}
		else {
			flag[i] = 0;
		}
		num = 0;
	}
	return num_width;
}

void pic_cutting(Mat& mat1, Mat& pic_cutting, int** selection, int number) {
	int real_height = mat1.cols;
	IplImage pI_1 = mat1;
	IplImage pI_2;
	IplImage pI_3;
	CvScalar s;

	pic_cutting = cv::Mat(selection[number][1] - selection[number][0], real_height, CV_8UC3, 1);
	pI_2 = pic_cutting;

	for (int i = selection[number][0]; i < selection[number][1]; i++) {
		for (int j = 0; j<real_height; j++) {
			s = cvGet2D(&pI_1, i, j);
			cvSet2D(&pI_2, i - selection[number][0], j, s);
		}
	}
}

void pic_cutting_1(Mat& mat1, Mat& mat2, Point s1, Point s2) {
	int i, j;
	IplImage pI_1;
	IplImage pI_2;
	CvScalar s;

	mat2 = cv::Mat(s2.y - s1.y, s2.x - s1.x, CV_8UC3, 1);
	pI_1 = mat1;
	pI_2 = mat2;

	for (i = s1.y; i < s2.y; i++) {
		for (j = s1.x; j<s2.x; j++) {
			s = cvGet2D(&pI_1, i, j);
			cvSet2D(&pI_2, i - s1.y, j - s1.x, s);
		}
	}
}

int** car_License_box(Mat& mat1, Mat& mat2, int* number) {
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Point s1, s2;
	int width_1, height_1;
	int width = mat1.rows;
	int height = mat1.cols;
	int sum = 0;

	int morph_elem = 3;
	int morph_size = 3;

	int** a = (int**)malloc(width * sizeof(int*));

	//腐蚀
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(-1, -1));
	dilate(mat1, mat1, element);

	/// 找到轮廓
	findContours(mat1, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	/// 画多边形轮廓 + 包围的矩形框 + 圆形框
	mat2 = Mat::zeros(mat1.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		s1 = boundRect[i].tl();
		s2 = boundRect[i].br();
		height_1 = s2.x - s1.x;
		width_1 = s2.y - s1.y;

		if ((height_1 >(3 * width_1)) && (width_1 > (width / 2))) {
			a[sum] = (int*)malloc(4 * sizeof(int));
			a[sum][0] = s1.x;
			a[sum][1] = s1.y;
			a[sum][2] = s2.x;
			a[sum][3] = s2.y;
			sum += 1;
		}
	}
	*number = sum;
	return a;
}

int box_selection(Mat& mat1) {
	int width_1, height_1;
	int width = mat1.rows;
	int height = mat1.cols;
	int i, j;

	IplImage pI_1 = mat1;

	CvScalar s;
	int find_blue = 0;
	int blueToWhite = 0;
	int sum = 0;

	for (i = 0; i<width; i++) {
		find_blue = 0;
		blueToWhite = 0;
		for (j = 0; j<height; j++) {
			s = cvGet2D(&pI_1, i, j);
			if ((s.val[0] - s.val[1] > 10) && (s.val[0] - s.val[2] > 10) && (s.val[1] < 150) && (s.val[2] < 150)) {
				find_blue = 1;
			}
			else if ((s.val[1] > 150) && (s.val[2] > 150) && (s.val[0] > 150) && (find_blue == 1)) {
				blueToWhite += 1;
				find_blue = 0;
			}
		}
		if (blueToWhite > 5) {
			sum += 1;
		}
	}
	return sum;
}

//normal.cpp-end




int main(int argc, char** argv) {
	Mat img, img_2, img_3, img_4, img_5, img_w;
	IplImage pI_1;
	IplImage pI_2;
	int width, reWidth = 30, wWidth = 20, pic_width;//reWidth=30
	int height, reHeight = 100, wHeight = 20;//reHeight=100
	char str[2];
	int i = 0, j = 0, k;
	int threshold = 0, pic_ArrNumber, tmp;
	//int vArr[reHeight];
	int vArr[100];
	int **pic_Arr;
	CvScalar s1;
	float percentage = 0.0;

	//if (argc < 2) {
	//	DE("Please input argv[1]\n");
	//	return -1;
	//}
	//img = cv::imread(argv[1]);
	img = cv::imread("num1.jpg");
	namedWindow(str);
	imshow(str, img);

	width = img.rows;
	height = img.cols;

	pic_gray(img, img_2);
	threshold = histogram_Calculate(img_2, 5);
	DE("threshold:%d\n", threshold);

	pic_Thresholding(img_2, threshold);
	sprintf(str, "%d", i + 1);
	namedWindow(str);
	imshow(str, img_2);


	//上下边缘分离-begin
	detectionChange(img_2, img_3, 7);

	sprintf(str, "%d", i + 2);
	namedWindow(str);
	imshow(str, img_3);
	//上下边缘分离-end

	//字符分割-begin
	
	//carCard_Resize(img_3, img_4, reWidth, reHeight);
	carCard_Resize(img_3, img_4, 60, 200);
	pic_Thresholding(img_4, 60);
	pI_1 = img_4;

	verProjection_calculate(img_4, vArr, reHeight);
	pic_Arr = verProjection_cut(vArr, reHeight, &pic_ArrNumber);

	for (i = 0; i< pic_ArrNumber; i++) {
		printf("pic_Arr[%d]:%d, %d\n", i, pic_Arr[i][0], pic_Arr[i][1]);
	}


	//后续处理-begin
	for (i = 0; i< pic_ArrNumber; i++) {
		pic_width = pic_Arr[i][1] - pic_Arr[i][0];
		if (pic_width < 3) {
			continue;
		}

		img_5 = cv::Mat(reWidth, pic_Arr[i][1] - pic_Arr[i][0], CV_8UC1, 1);
		pI_2 = img_5;
		for (j = 0; j<reWidth; j++) {
			for (k = pic_Arr[i][0]; k<pic_Arr[i][1]; k++) {
				s1 = cvGet2D(&pI_1, j, k);
				cvSet2D(&pI_2, j, k - pic_Arr[i][0], s1);
			}
		}
		percentage = pixelPercentage(img_5);
		if (percentage < 0.1) {
			continue;
		}
		if (pic_width < 6) {
			printf("the %d is 1\n", i);
			continue;
		}
		carCard_Resize(img_5, img_w, wWidth, wHeight);
		pic_Thresholding(img_w, 60);
		sprintf(str, "%d", i + 10);
		namedWindow(str);
		imshow(str, img_w);
	}
	//后续处理-end
	sprintf(str, "%d", i + 3);
	namedWindow(str);
	imshow(str, img_4);
	//字符分割-end


	
	waitKey(0);
	return 0;
}