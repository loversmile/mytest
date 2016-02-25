/*************************************************************************
	> File Name: ssss.c
	> Author: 
	> Mail: 
	> Created Time: 2014年12月17日 星期三 18时36分21秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    char input[64] = "{\"response\" = {\"chanllege\":\"asdjofhsdajkfh\"}}";
    char aa[64] = {0};
    sscanf(input, "{\"response\" = {\"chanllege\":\"%[^\"]\"}}", aa);
    printf("aa = %s\n", aa);
}
