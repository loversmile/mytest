/*************************************************************************
	> File Name: nofree.c
	> Author: 
	> Mail: 
	> Created Time: 2015年08月06日 星期四 17时07分27秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>

int main()
{
    char *aa = malloc(256);
    free(aa);
    return 0;
}
