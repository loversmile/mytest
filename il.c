/*************************************************************************
	> File Name: il.c
	> Author: 
	> Mail: 
	> Created Time: 2015年08月22日 星期六 14时12分27秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    double li = 0.0276;
    double all = 126000.0;
    double yue = 3500.0;
    double sum = 0.0;
    do{
        sum += all*li/12;
        all = all - yue;
        printf("all = %f\n", all);
    }while(all > -0.1);
    printf("%f\n", sum);
    printf("%f\n", sum+126000.0);
    double lilv = 0.0;
    int nian = 0;
    double qian = 0.0;
    double lixi = 0.0;
    printf("Please input the year:  ");
    scanf("%d", &nian);
    printf("Please input the lilv:  ");
    scanf("%lf", &lilv);
    printf("Please input the cunkuan:  ");
    scanf("%lf", &qian);
    int zongyue = 0;
    double tmp_qian = 0.0;
    tmp_qian = qian;
    zongyue = nian * 12;
    printf("nian = %d, lilv = %f, qian = %f\n", nian,lilv,qian);
    do{
        lixi += tmp_qian * lilv / 12;
        tmp_qian = tmp_qian + qian;
        zongyue = zongyue - 1;
        printf("%lf\n", tmp_qian);

    }while(zongyue);
    printf("lixi = %f\n", lixi);
    printf("benxi = %f\n", qian * nian * 12 + lixi);
    return 0;
}
