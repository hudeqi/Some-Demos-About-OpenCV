#include "KMeans.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <math.h>  
#include <string.h>  
#include <opencv/cv.h>  
#include <stdio.h>
#include <iostream>
#include<fstream>
#include<iomanip>

using namespace std;
using namespace cv;



float getDistance(int point1, int point2)
{
	float d;
	d = fabs(point1 - point2);
	return d;
}

/// ����ÿ���ص����ĵ�  
void getMean(int Num, int *tempy, int K, int *center, float *means)
{
	float tep;
	int i, j, count = 0;
	for (i = 0; i < K; ++i)
	{
		count = 0;
		tep = 0.0;   /// ÿ���һ���ص����ĵ�ֵ����0   
		for (j = 0; j < Num; ++j)
		{
			if (i == center[j])
			{
				count++;
				tep += tempy[j];
			}
		}
		tep /= count;
		means[i] = tep;
	}
}

/// ����ƽ������  
float getE(int Num, int *tempy, int K, int *center, float *means)
{
	int i, j;
	float cnt = 0.0, sum = 0.0;
	for (i = 0; i < K; ++i)
	{
		for (j = 0; j < Num; ++j)
		{
			if (i == center[j])
			{
				cnt = (tempy[j] - means[i]) * (tempy[j] - means[i]);
				sum += cnt;
			}
		}
	}
	return sum;
}

/// ��N�������  
void cluster(int Num, int *tempy, int K, int *center, float *means)
{
	int i, j, q;
	float min;
	float ds[1000][3];
	for (i = 0; i < 1000; i++)
		for (j = 0; j < 3; j++)
			ds[i][j] = 0.0;
	for (i = 0; i < Num; ++i)
	{
		min = 999999.0;
		for (j = 0; j < K; ++j)
		{
			ds[i][j] = getDistance(tempy[i], means[j]);
		}
		for (q = 0; q < K; ++q)
		{
			if (ds[i][q] < min)
			{
				min = ds[i][q];
				center[i] = q;
			}
		}
	}
}

int cluster_p(int Num, int *tempy, int K, int *center, float *means)
{
	int i, j, n = 0;
	float temp1;
	float temp2, t;
	//mean = (float *)malloc(sizeof(mean) * K);
	//float *mean = new float[K];
	int initMax = -1;
	int initMin = 1000000;
	for (i = 0; i < Num; i++)
	{
		if (tempy[i] > initMax)
			initMax = tempy[i];
		if (tempy[i] < initMin)
			initMin = tempy[i];
	}
	means[0] = initMax;      /// ��ʼ��k�����ĵ�
	means[1] = (initMax + initMin) / 2;
	means[2] = initMin;
	//float means[3];
	/*for (i = 0; i < K; i++)
	means[i] = tempy[i];*/

	cluster(Num, tempy, K, center, means);          /// ��һ�θ���Ԥ���k������о���  
	temp1 = getE(Num, tempy, K, center, means);        ///  ��һ��ƽ�����  
	n++;                   ///  n�����γ����յĴ����˶��ٴ�  

	//printf("The E1 is: %f\n\n", temp1);

	getMean(Num, tempy, K, center, means);
	cluster(Num, tempy, K, center, means);
	temp2 = getE(Num, tempy, K, center, means);        ///  ���ݴ��γ��µ����ĵ㣬�������ƽ�����  
	n++;

	//printf("The E2 is: %f\n\n", temp2);
	int iter = 1000;
	while (iter--)   ///  �Ƚ�����ƽ����� �ж��Ƿ���ȣ�����ȼ�������  
	{
		temp1 = temp2;
		getMean(Num, tempy, K, center, means);
		cluster(Num, tempy, K, center, means);
		temp2 = getE(Num, tempy, K, center, means);
		n++;
		//printf("The E%d is: %f\n", n, temp2);
	}

	//printf("The total number of cluster is: %d\n\n", n);  /// ͳ�Ƴ���������  

	return 0;
}