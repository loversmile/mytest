#include <stdlib.h>
#include <stdio.h>

typedef enum _TS{
    ZERO = 0,
    ONE,
    TWO,
    THREE
}TS;

static int ThreadRun(int * aa)
{
    printf("sajdlfjsdklafjsd\n");
}

int main()
{
    TS type;
    int *index = (int *)malloc(sizeof(int));
    *index = ONE;
    printf("*aa = %d \n", *index);
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_attr_setstacksize(&attr, 512 * 1024);
    int ret = pthread_create(&thread, &attr, (void *)ThreadRun, (void *)index);
    sleep(1);
    printf("ret = %d\n", ret);
    return 0;
}
