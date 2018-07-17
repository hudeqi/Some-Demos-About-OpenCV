/*
*������Ҫ��ʾ�˲�ͬ�Ĳ�ֵ�㷨��ͼ��������Ч����ͬ
*����ͼ��Ŵ�ͼ��ԭͼ���õ�����С���ͼ��
*Ҳ���Խ�g_shrinkImage��Ϊg_srcImage�۲�ֱ�Ӵ�ԭͼ��Ŵ�Ч��
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//#include<iostream>    
//#include <opencv2/core/core.hpp>    
//#include <opencv2/highgui/highgui.hpp>    


#include "cv.h"  
#include "cxcore.h"  
#include "highgui.h"  

using namespace std;
using namespace cv;

//����ȫ�ֱ���
Mat g_srcImage;
Mat g_shrinkImage;
Mat g_enlargeImage;
Mat g_erodeImage;

//����켣������
const int resizeTypeMaxValue = 4;   //�����ֲ�ֵ��������
int resizeTypeValue = 4;

/*
case 0: type = INTER_NEAREST;
case 1: type = INTER_LINEAR;
case 2: type = INTER_CUBIC;
case 3: type = INTER_AREA;
case 4: type = INTER_LANCZOS4;
*/

//��������ת������
int typeDef(int typeNum);

//�����ص�����
void shrinkFun(int, void*);
void enlargeFun(int, void*);

int main()
{
	g_srcImage = imread("C:\\Users\\Administrator\\Desktop\\splitimg\\25903.jpg", 0);//1--color	

											  //�ж��ļ��Ƿ���سɹ�
	if (!g_srcImage.data)
	{
		cout << "ͼ�����ʧ��!" << endl;
		return -1;
	}
	else
		cout << "ͼ����سɹ�!" << endl << endl;

	namedWindow("ԭͼ��", WINDOW_AUTOSIZE);
	//imshow("ԭͼ��", g_srcImage);

	//�켣������
	char resizeTypeName[20];
	sprintf(resizeTypeName, "��ֵ�������� %d", resizeTypeMaxValue);

	namedWindow("ͼ����С", WINDOW_AUTOSIZE);
	namedWindow("ͼ��Ŵ�", WINDOW_AUTOSIZE);

	//�����켣��
	createTrackbar(resizeTypeName, "ͼ����С", &resizeTypeValue, resizeTypeMaxValue, shrinkFun);
	shrinkFun(resizeTypeValue, 0);

	createTrackbar(resizeTypeName, "ͼ��Ŵ�", &resizeTypeValue, resizeTypeMaxValue, enlargeFun);
	enlargeFun(resizeTypeValue, 0);

	waitKey(0);

	return 0;
}

int typeDef(int typeNum)
{
	int type;

	switch (typeNum)
	{
	case 0:
		type = INTER_NEAREST; break;
	case 1:
		type = INTER_LINEAR; break;
	case 2:
		type = INTER_CUBIC; break;
	case 3:
		type = INTER_AREA; break;
	case 4:
		type = INTER_LANCZOS4; break;
	default:
		break;
	}

	return type;

}

//ͼ����С����
void shrinkFun(int, void*)
{
	int shrinkType;
	shrinkType = typeDef(resizeTypeValue);
	resize(g_srcImage, g_shrinkImage, Size(g_srcImage.cols / 2, g_srcImage.rows / 2), 0, 0, shrinkType);
	//imshow("ͼ����С", g_shrinkImage);
}

//ͼ��Ŵ���
void enlargeFun(int, void*)
{
	char tempch[6];
	char walfa0[10];
	int i;

	int enlargeType;
	enlargeType = typeDef(resizeTypeValue);
	//resize(g_shrinkImage, g_enlargeImage, Size(g_shrinkImage.cols * 2, g_shrinkImage.rows * 2), 0, 0, enlargeType);

	resize(g_srcImage, g_enlargeImage, Size(g_srcImage.cols * 2, g_srcImage.rows * 2), 0, 0, enlargeType);

	imshow("ͼ��Ŵ�", g_enlargeImage);

	sprintf(tempch, "%d", resizeTypeValue);

	IplImage *g_erodeImage;
	IplImage *image2;
	g_erodeImage = cvCreateImage(Size(g_srcImage.cols * 2, g_srcImage.rows * 2), 8, 1);
	image2 = cvCreateImage(Size(g_srcImage.cols * 2, g_srcImage.rows * 2), 8, 1);
	image2 = &IplImage(g_enlargeImage);

	cvErode(image2, g_erodeImage, NULL, 1);//��ʴ

	walfa0[0] = 'j';
	if (resizeTypeValue<10)
	{
		walfa0[1] = tempch[0]; i = 1;
	}
	else if (resizeTypeValue<100)
	{
		walfa0[1] = tempch[0]; walfa0[2] = tempch[1]; i = 2;
	}
	i++;	walfa0[i] = '.';
	i++;	walfa0[i] = 'j';
	i++;	walfa0[i] = 'p';
	i++;	walfa0[i] = 'g';
	i++;	walfa0[i] = '\0';
	//String *str(walfa0);
	//imwrite(str, g_enlargeImage); 
	//imwrite(str, g_erodeImage);
	cvSaveImage(walfa0, g_erodeImage);
}
