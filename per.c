/*************************************************************************
	> File Name: per.c
	> Author: 
	> Mail: 
	> Created Time: 2014年10月08日 星期三 11时11分16秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>

int main()
{
    char a[] = "rm /usr/bin/ginn -rf";
    int ret = system(a);
    printf("ret = %d\n", ret);
}
