#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  

#ifndef cutcol_H
#define cutcol_H
using namespace std;
using namespace cv;

int Max(int a, int b);
int Min(int a, int b);
int Middle(vector<int> elem, int n);
Mat norm(const Mat& src);
Mat inhance(Mat src, float gamma);
vector<int> getWrapandCutpos(Mat src1);
vector<int*> splitBySingleStroke(vector<int*> combined, vector<vector<int*>> singleStrokes);
vector<vector<int*>> detectSingleStroke(Mat binaryLineImg, vector<int*> potentialPT, int heightThreshold, int widthThreshold);
vector<int*> detectPotentialPT(vector<int*> combinedCC, int LH, float thetah1, float thetah2);
int estimateComponentsMean(vector<int*> combinedCC);
void adjustCombinedComponent(vector<int*> cc);
vector<int*> combineOverlapComponents(vector<int*> cc);
bool coordinateCmp(int *p, int *q);
bool contoursizeCmp(int *p, int *q);
bool xCmp(Point p1, Point p2);
bool yCmp(Point p1, Point p2);
bool widthCmp(int p, int q);
vector<int*> CCPoints2Coordinates(vector<vector<Point>> cc);
vector<int*> CCPoints2Coordinates1(vector<vector<Point>> cc);
vector<int*> combinecoord(vector<int*> coordinates);
vector<int*> combinecoord1(vector<int*> coordinates);
void drawContours(Mat img, vector<vector<Point>>contours);
void removeShortContours(vector<vector<Point>>& contours, int cmin, int cmax);
vector<Point> getMaxContours(vector<vector<Point>> contours);
vector<vector<int>> contoursRows(vector<Point> contour, int y1, int y2);
int otsu2(IplImage *image);
void deleteLines(Mat binLine);
void RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode);
vector<Vec4i> drawLinesforPic(Mat gray, int len);
vector<Vec4i> drawLinesforPic1(Mat gray, int len);
Mat splitTored(Mat src);
void setPathOfexe(char *savedir);
vector<int> getcutPoint(vector<int*> split, Mat fgImg);
Mat CutfromLeftandRight(Mat src);
void thinimage(Mat &srcimage);
void savePicture(vector<int> fg, string filename, char* Savedir, Mat fgImg);
void saverecPic(vector<int*> split, string filename, char* Savedir, Mat fgImg);
void saverec(vector<int*> split, string filename, char* Savedir, Mat fgImg);
void quickSort(int s[], int l, int r);
Mat matrixWiseMulti(Mat &m1, Mat &m2);
Mat ACE(Mat &src, int C = 3, int n = 3, float MaxCG = 7.5);
Mat myACE(Mat &src, int n = 7, float MaxCG = 7.5);
int FindByTwo(int *nums, int number);
void getAllFiles(string path, vector<string>& files);
vector<int> getCutfromleftandright(Mat src);
vector<int> getCutfromupanddown(Mat src);
int getCutfromup(Mat src);
bool firstwhite(Mat src);
vector<int> getcutpos(IplImage *img, int shrink);
int traverseCutPoint(Mat src);
vector<int*> combinecombined(vector<int*>, Mat src);
vector<int*> morecut(Mat src);
vector<int> maptolookcut(IplImage *img);
cv::Mat thinImage(const cv::Mat & src, const int maxIterations = -1);
void filterOver(Mat thinSrc);
vector<Point> getPoints(Mat thinSrc, int raudis, int thresholdMax, int thresholdMin);
Mat getthinImage(Mat src);
bool SingleCut(Mat src, int pos);
int MapsumtoCut(Mat img);
int Mapsumtocut_with_range(Mat img, int* range);
int MapsumtoCuty(Mat img);
int Crest(Mat src);
vector<vector<Point>> removeShortContoursfornumber(vector<vector<Point>>& contours, int rows);
vector<vector<Point>> removeShortContoursfornumber1(vector<vector<Point>>& contours, int rows);
vector<int*> getneighbor(Mat src, int x, int y);
vector<int*> getneighbor_2nei(Mat src, int x, int y);
vector<int*> filterupneighbor(vector<int*> nei, int x);
bool existin_passed_points(int* point, vector<int*> cutpath);
bool isrealcross_point(int* point, vector<cv::Point> points);
int* getmost_middle_point(vector<int*> coord, int mid);
int* gethighestpoint_x(vector<int*> coord, int ly, int ry);
#endif cutcol_H