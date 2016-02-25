/*************************************************************************
	> File Name: thread.c
	> Author: 
	> Mail: 
	> Created Time: 2015年09月30日 星期三 11时33分46秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>

static pthread_mutex_t testlock;
char *pp;

void *test1(void *var)
{
    pthread_mutex_lock(&testlock);
    sleep(1);
    pp = "123";
    printf("11111------\n");
    pthread_mutex_unlock(&testlock);
    pthread_mutex_unlock(&testlock);
}

void *test2(void *var)
{
    pthread_mutex_lock(&testlock);
    sleep(1);
    pp = "456";
    printf("22222------\n");
    pthread_mutex_unlock(&testlock);
    pthread_mutex_unlock(&testlock);
}

void *test3(void *var)
{
    pthread_mutex_lock(&testlock);
    sleep(1);
    pp = "789";
    printf("33333------\n");
    pthread_mutex_unlock(&testlock);
    pthread_mutex_unlock(&testlock);
}

int main()
{
    pthread_t pid1;
    pthread_t pid2;
    pthread_t pid3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_mutex_init(&testlock, NULL);
    pthread_create(&pid1, &attr, test1, NULL);
    pthread_create(&pid2, &attr, test2, NULL);
    pthread_create(&pid3, &attr, test3, NULL);
   // pthread_join(pid1,NULL); 
   // pthread_join(pid2,NULL); 
   // pthread_join(pid3,NULL); 
//    sleep(10);
    sleep(4);
    printf("pp = %s\n", pp);
    pthread_mutex_destroy(&testlock);
}
