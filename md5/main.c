/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sun 06 Mar 2016 06:27:32 PM PST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

char *MD_5(unsigned char *input)
{
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    unsigned char decrypt[16];
    MD5Update(&md5,input,strlen((char *)input));
    MD5Final(&md5,decrypt);
    char buf[40] = {0};
    for (i = 0; i < 16; i++)
    {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                "%02x", decrypt[i]);
    }
    return strdup(buf);
}

int main(int argc, char *argv[])
{
    char *p = MD_5(argv[1]);
    printf("%s\n", p);
}
#if 0
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
    char buf[40] = {0};
    for(i=0;i<16;i++)
    {
        printf("%02x",decrypt[i]);
        snprintf(buf + strlen(buf), sizeof(buf),"%02x", decrypt[i]);
    }
    printf("\n---%s---\n",buf);
    getchar();                                                                                                                                          
    return 0;
}
#endif
