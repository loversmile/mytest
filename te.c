/*************************************************************************
	> File Name: te.c
	> Author: jklou
	> Mail: 
	> Created Time: 2015年11月10日 星期二 09时09分52秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MUL(a,b) a*b
#define ADD(a,b) a+b

int main()
{
    char *buf1 = "abcd\0";
    char buf2[] = "cdefgh\n";
    printf("%d\n", sizeof(buf1));
    printf("%d\n", strlen(buf1));
    printf("%d\n", sizeof(buf2));
    printf("%d\n", strlen(buf2));
    printf("%d---%d\n", MUL(2,2)*ADD(3,4), MUL(ADD(2,2),ADD(3,4)));
}
