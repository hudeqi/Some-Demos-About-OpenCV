#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <math.h>  
#include <time.h>  
  
#define K 3  
#define N 1111 
int center[N];  ///  �ж�ÿ���������ĸ���  
float mean[K];  ///  ����ÿ���ص����ĵ�  
  
float getDistance(int point1, int point2)  
{  
    float d;  
    d = abs(point1 - point2);  
    return d;  
}  
  
/// ����ÿ���ص����ĵ�  
void getMean(int center[N])  
{  
    float tep;  
    int i, j, count = 0;  
    for(i = 0; i < K; ++i)  
    {  
        count = 0;  
        tep = 0.0;   /// ÿ���һ���ص����ĵ�ֵ����0   
        for(j = 0; j < N; ++j)  
        {  
            if(i == center[j])  
            {  
                count++;  
                tep += tempy[j];
            }  
        }  
        tep /= count;  
        mean[i] = tep;  
    }  
    for(i = 0; i < K; ++i)  
    {  
        printf("The new center point of %d is : \t( %f, %f )\n", i+1, mean[i], mean[i]);  
    }  
}  
  
/// ����ƽ������  
float getE()  
{  
    int i, j;  
    float cnt = 0.0, sum = 0.0;  
    for(i = 0; i < K; ++i)  
    {  
        for(j = 0; j < N; ++j)  
        {  
            if(i == center[j])  
            {  
                cnt = (tempy[j] - mean[i]) * (tempy[j] - mean[i]);  
                sum += cnt;  
            }  
        }  
    }  
    return sum;  
}  
  
/// ��N�������  
void cluster()  
{  
    int i, j, q;  
    float min;  
    float distance[N][K];  
    for(i = 0; i < N; ++i)  
    {  
        min = 999999.0;  
        for(j = 0; j < K; ++j)  
        {  
            distance[i][j] = getDistance(tempy[i], tempy[j]);  
        }  
        for(q = 0; q < K; ++q)  
        {  
            if(distance[i][q] < min)  
            {  
                min = distance[i][q];  
                center[i] = q;  
            }  
        }  
        printf("( %.0f, %.0f )\t in cluster-%d\n", point[i].x, point[i].y, center[i] + 1);  
    }  
    printf("-----------------------------\n");  
}  
  
int cluster_p()  
{  
    int i, j, n = 0;  
    float temp1;  
    float temp2, t;  
    
    mean[0] = tempy[0];      /// ��ʼ��k�����ĵ�   
    mean[1] = tempy[3];  
    mean[2] = tempy[6];  
  
    cluster();          /// ��һ�θ���Ԥ���k������о���  
    temp1 = getE();        ///  ��һ��ƽ�����  
    n++;                   ///  n�����γ����յĴ����˶��ٴ�  
  
    printf("The E1 is: %f\n\n", temp1);  
  
    getMean(center);  
    cluster();  
    temp2 = getE();        ///  ���ݴ��γ��µ����ĵ㣬�������ƽ�����  
    n++;  
  
    printf("The E2 is: %f\n\n", temp2);  
  
    while(fabs(temp2 - temp1) != 0)   ///  �Ƚ�����ƽ����� �ж��Ƿ���ȣ�����ȼ�������  
    {  
        temp1 = temp2;  
        getMean(center);  
        cluster();  
        temp2 = getE();  
        n++;  
        printf("The E%d is: %f\n", n, temp2);  
    }  
  
    printf("The total number of cluster is: %d\n\n", n);  /// ͳ�Ƴ���������  
    system("pause");  
    return 0;  
}  