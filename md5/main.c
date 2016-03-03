/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Wed 02 Mar 2016 07:30:20 PM PST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
int main(int argc, char *argv[])
{
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    //unsigned char encrypt[] = "userid2016030309555112345";//"admin";//21232f297a57a5a743894a0e4a801fc3
    unsigned char decrypt[16];
    MD5Update(&md5,argv[1],strlen((char *)argv[1]));
    MD5Final(&md5,decrypt);
    printf("加密前:%s\n加密后16位:",argv[1]);
    for(i=4;i<12;i++)
    {
        printf("%02x",decrypt[i]);
    }

    printf("\n加密前:%s\n加密后32位:",argv[1]);  
    for(i=0;i<16;i++)
    {
        printf("%02x",decrypt[i]);
    }
  
    getchar();
 
    return 0;
}
