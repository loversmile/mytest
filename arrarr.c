/*************************************************************************
	> File Name: arrarr.c
	> Author: 
	> Mail: 
	> Created Time: 2015年10月12日 星期一 19时47分12秒
 ************************************************************************/

#include<stdio.h>


int main()
{
    char aa[10][64]={"qwe","zxv"};
    //printf("%d\n", strlen(aa));
    

    int i = 0;
    for ( i = 0; i < 10; i++ ){
        if(aa[i] != NULL && aa[i][0] != '\0')
        printf("aa[i%d] = %s\n",i,aa[i]);
    }

