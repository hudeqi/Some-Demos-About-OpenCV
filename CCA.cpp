#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <stack>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//联通区分析，种子填充法
void icvprCcaBySeedFill(const cv::Mat& _binImg, cv::Mat& _lableImg)
{
	// connected component analysis (8-component)
	// use seed filling algorithm
	// 1. begin with a foreground pixel and push its foreground neighbors into a stack;
	// 2. pop the top pixel on the stack and label it with the same label until the stack is empty
	// 
	// foreground pixel: _binImg(x,y) = 1
	// background pixel: _binImg(x,y) = 0


	if (_binImg.empty() ||
		_binImg.type() != CV_8UC1)
	{
		return;
	}

	_lableImg.release();
	_binImg.convertTo(_lableImg, CV_32SC1);

	int label = 1;  // start by 2

	int rows = _binImg.rows - 1;
	int cols = _binImg.cols - 1;
	for (int i = 1; i < rows - 1; i++)
	{
		int* data = _lableImg.ptr<int>(i);
		for (int j = 1; j < cols - 1; j++)
		{
			if (data[j] == 1)
			{
				std::stack<std::pair<int, int>> neighborPixels;
				neighborPixels.push(std::pair<int, int>(i, j));     // pixel position: <i,j>
				++label;  // begin with a new label
				while (!neighborPixels.empty())
				{
					// get the top pixel on the stack and label it with the same label
					std::pair<int, int> curPixel = neighborPixels.top();
					int curX = curPixel.first;
					int curY = curPixel.second;
					_lableImg.at<int>(curX, curY) = label;

					// pop the top pixel
					neighborPixels.pop();

					// push the 4-neighbors (foreground pixels)
					if (_lableImg.at<int>(curX, curY - 1) == 1)
					{// left pixel
						neighborPixels.push(std::pair<int, int>(curX, curY - 1));
					}
					if (_lableImg.at<int>(curX, curY + 1) == 1)
					{// right pixel
						neighborPixels.push(std::pair<int, int>(curX, curY + 1));
					}
					if (_lableImg.at<int>(curX - 1, curY) == 1)
					{// up pixel
						neighborPixels.push(std::pair<int, int>(curX - 1, curY));
					}
					if (_lableImg.at<int>(curX + 1, curY) == 1)
					{// down pixel
						neighborPixels.push(std::pair<int, int>(curX + 1, curY));
					}
					//if (_lableImg.at<int>(curX - 1, curY - 1) == 1)
					//{// left pixel
					//	neighborPixels.push(std::pair<int, int>(curX, curY - 1));
					//}
					//if (_lableImg.at<int>(curX + 1, curY + 1) == 1)
					//{// right pixel
					//	neighborPixels.push(std::pair<int, int>(curX, curY + 1));
					//}
					//if (_lableImg.at<int>(curX - 1, curY + 1) == 1)
					//{// up pixel
					//	neighborPixels.push(std::pair<int, int>(curX - 1, curY));
					//}
					//if (_lableImg.at<int>(curX + 1, curY - 1) == 1)
					//{// down pixel
					//	neighborPixels.push(std::pair<int, int>(curX + 1, curY));
					//}
				}
			}
		}
	}
}