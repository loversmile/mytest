/*************************************************************************
	> File Name: lenstr.c
	> Author: 
	> Mail: 
	> Created Time: 2015年09月15日 星期二 17时35分03秒
 ************************************************************************/

#include<stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char a[128] = {0};
    int len = 10;
    int i = 0;
    for (i = 0; i < len; ++i){
        snprintf(a + strlen(a), sizeof(a), "-");
    }
    printf("a = %s\n", a);

}
