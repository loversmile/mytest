/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sun 27 Mar 2016 08:00:16 PM PDT
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include "memwatch.h"

int main(int argc,char **argv)
{
    int i = 0;
    char *p;
    mwInit();

    p = malloc(100);
    p = malloc(200);
    free(p);

    for(i=0;i<5;i++)
    {
        p = malloc(50);
        if(p == NULL)
        {
            printf("can't malloc memory for test,num:%d\n",i);
            continue;
        }
        if((i%2) == 0)
        {
            free(p);
            p = NULL;
        }
    }
    mwTerm();
    return;
}
