/*************************************************************************
	> File Name: 0.c
	> Author: 
	> Mail: 
	> Created Time: 2014年10月14日 星期二 09时34分31秒
 ************************************************************************/

#include<stdio.h>
#include <string.h>

int main()
{
    char a[128] = {0};
    printf("%d\n", sizeof(a));
    printf("%d\n", strlen(a));
    char *aa = NULL;
    printf("%d\n",strlen(aa));
}
