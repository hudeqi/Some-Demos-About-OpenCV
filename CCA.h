#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <stack>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#ifndef CCA_H
#define CCA_H

void icvprCcaBySeedFill(const cv::Mat& _binImg, cv::Mat& _lableImg);
#endif CCA_H
