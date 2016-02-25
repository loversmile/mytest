/*************************************************************************
	> File Name: hong.c
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2016 11:46:32 PM PST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#define f(a,b) a##b
#define g(a)  #a
#define h(a) g(a)
 
int main()
{
    printf("%s\n", h(f(1,2)));
    printf("%s\n", g(f(1,2)));
    return 0;
}
