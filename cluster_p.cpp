#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <math.h>  
#include <time.h>  
  
#define K 3  
#define N 1111 
int center[N];  ///  判断每个点属于哪个簇  
float mean[K];  ///  保存每个簇的中心点  
  
float getDistance(int point1, int point2)  
{  
    float d;  
    d = abs(point1 - point2);  
    return d;  
}  
  
/// 计算每个簇的中心点  
void getMean(int center[N])  
{  
    float tep;  
    int i, j, count = 0;  
    for(i = 0; i < K; ++i)  
    {  
        count = 0;  
        tep = 0.0;   /// 每算出一个簇的中心点值后清0   
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
  
/// 计算平方误差函数  
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
  
/// 把N个点聚类  
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
    
    mean[0] = tempy[0];      /// 初始化k个中心点   
    mean[1] = tempy[3];  
    mean[2] = tempy[6];  
  
    cluster();          /// 第一次根据预设的k个点进行聚类  
    temp1 = getE();        ///  第一次平方误差  
    n++;                   ///  n计算形成最终的簇用了多少次  
  
    printf("The E1 is: %f\n\n", temp1);  
  
    getMean(center);  
    cluster();  
    temp2 = getE();        ///  根据簇形成新的中心点，并计算出平方误差  
    n++;  
  
    printf("The E2 is: %f\n\n", temp2);  
  
    while(fabs(temp2 - temp1) != 0)   ///  比较两次平方误差 判断是否相等，不相等继续迭代  
    {  
        temp1 = temp2;  
        getMean(center);  
        cluster();  
        temp2 = getE();  
        n++;  
        printf("The E%d is: %f\n", n, temp2);  
    }  
  
    printf("The total number of cluster is: %d\n\n", n);  /// 统计出迭代次数  
    system("pause");  
    return 0;  
}  