/*************************************************************************
	> File Name: fupt.c
	> Author: jklou
	> Mail: 
	> Created Time: 2016年01月06日 星期三 15时33分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_st {
    int (*func)(int, int);
}; 

static int func1(int a, int b)
{
    printf("a + b = %d\n", a + b);
    return 0;
}

struct test_st test = {
    .func = func1,
};

int main()
{
    struct test_st *aa = &test;
    aa->func(1,2);
    return 0;
}
