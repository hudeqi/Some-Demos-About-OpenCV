#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "cutcol.h"

using namespace std;
using namespace cv;

int main_vgfbn()
{
	Mat src = imread("C:\\Users\\Administrator\\Desktop\\wrong_src\\12.png");
	
	//int th = 90; 
	Mat binary;
	vector<Mat> channels;
	split(src, channels);
	Mat red = channels[2];
	Mat blue = channels[0];
	Mat green = channels[1];

	Mat red_binary;
	Mat d = red.clone();
	IplImage *temp = &IplImage(d);
	int thres = otsu2(temp) - 10;
	threshold(red, red_binary, thres, 255, CV_THRESH_BINARY);
	//threshold(red, red_binary, 0, 255, CV_THRESH_OTSU);
	//RemoveSmallRegion(red_binary, red_binary, 20, 1, 1);
	//imshow("src", src);
	//imshow("gray", gray);
	//imshow("binary", binary);
	imshow("red channel", red);
	//imshow("blue channel", blue);
	//imshow("green channel", green);
	imshow("red+binary", red_binary);

	//imwrite("C:\\Users\\Administrator\\Desktop\\12.png", red_binary);

	waitKey();
	return 0;
}

