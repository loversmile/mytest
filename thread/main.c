/*************************************************************************
    > File Name: main.c
    > Author: 
    > Mail: 
    > Created Time: Thu 24 Mar 2016 02:52:15 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sqlite3.h>

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int g_task = 1;

void *monitor_start(void *data)
{
    int i = 1;
 /*   while(1)
    {
        int a = access("/loverson/JKLOU/abc.txt", 0);

        printf("a = %d\n", a);
        if (0 == a) {
            g_task = 0;
            pthread_exit(NULL);
            break;
        }
        sleep(2);
        i = i + 1;
        if (i == 11) {
            g_task = 0;
            break;
        }
    }*/
    printf("start monitor\n");
    pthread_mutex_lock(&mut);
    while(i)
    {
        pthread_cond_wait(&cond,&mut);
        printf("wait\n");
        i = 0;
        printf("in while\n");
    }
    printf("out while\n");
    pthread_mutex_unlock(&mut);
    //g_task = 0;
}

void *signal_start(void *data)
{
    int j = 0;
    while(j < 20) {
        pthread_mutex_lock(&mut);
        j++;
        if (j == 10) {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mut);
        printf("not signal\n");
        sleep(1);
    }
    printf("after signal\n");
}

int main()
{
    pthread_t pid_t;
    pthread_t pid_x;
    pthread_rwlock_t p_lock;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize((&attr), 10240);
    pthread_attr_setdetachstate((&attr), PTHREAD_CREATE_DETACHED);
    pthread_rwlock_init(&p_lock, NULL);

    pthread_create(&pid_t, &attr, monitor_start, NULL);
    pthread_create(&pid_x, &attr, signal_start, NULL);

    while(g_task);
}

