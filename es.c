/*************************************************************************
	> File Name: /loverson/JKLOU/home/mylou/es.c
	> Author: jklou
	> Mail: 
	> Created Time: 2015年10月21日 星期三 12时00分16秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

char* escapeSpecificCharacter(const char *str,char ch)
{
    if(!str)
    {
        return NULL;
            
    }
    char *pos = NULL,*buf = NULL;
    int cnt = 0,length = 0;
    char *tmp = (char*)str;
    while(*tmp != '\0' && (pos = strchr(tmp,ch)))
    {
        cnt++;
        tmp = pos + 1;
    }
    length = strlen(str)+cnt+1;
    buf = (char *)calloc(length,sizeof(char));
    printf( "escapeSpecificCharacter : cnt = %d,calloc length = %d",cnt,length);
    tmp = buf;
    while(*str != '\0')
    {
        if(*str == ch || *str == '`')
        {
            *tmp = '\\';
            *(++tmp) = *str;
        }
        else
        {
            *tmp = *str;
        }
        str++;
        tmp++;
    }
    *tmp = '\0';
    return buf;
}

int main()
{
    printf("%d\n", (int)strlen("123\""));
}
