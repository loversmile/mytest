/*************************************************************************
	> File Name: math_jkl.c
	> Author: 
	> Mail: 
	> Created Time: 2014年09月10日 星期三 09时32分16秒
 ************************************************************************/

#include<stdio.h>

int plus(int a, int b)
{
    return a + b;
}

int minus(int a, int b)
{
    return a - b;
}

int multiply(int a, int b)
{
    return a * b;
}

int div(int a, int b)
{
    if ( b != 0 )
    {
        return a /b;
    }
    else 
    {
        return 0xFFFFFFFF;
    }
}

