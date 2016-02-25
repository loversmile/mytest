/*************************************************************************
	> File Name: st1.c
	> Author: 
	> Mail: 
	> Created Time: 2015年09月24日 星期四 14时06分29秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    fp = fopen("/etc/shadow", "r");
    if (fp == NULL){
        printf("FAILED!\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
