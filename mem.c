/*************************************************************************
	> File Name: mem.c
	> Author: 
	> Mail: 
	> Created Time: Tue 26 Jan 2016 12:32:34 AM PST
 ************************************************************************/

#include<stdio.h>
#include <string.h>
#include <stdlib.h>

struct att {
    int a;
    int b;
    int c;
};

int main()
{
    struct att tt; 
    struct att pp;
    tt.a = 10;
    tt.b = 11;
    tt.c = 12;
    pp.a = 10;
    pp.b = 11;
    pp.c = 13;
    if(!memcmp(&tt, &pp, sizeof(tt)))
        printf("-------");
    return 0;
}
