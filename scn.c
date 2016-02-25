/*************************************************************************
	> File Name: scn.c
	> Author: 
	> Mail: 
	> Created Time: 2015年08月03日 星期一 17时06分58秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    char aa[32] = {0};
    char bb[32] = {0};
    char cc[] = "a.c.c.v/255.255.255.0";
    int res = sscanf(cc, "%[^/]/%s", aa, bb);
    if (res == 1) {
        printf("aa = %s\n", aa);
    }
    else if (res == 2) {
        printf("aa = %s, bb = %s\n", aa, bb);
    }
}
