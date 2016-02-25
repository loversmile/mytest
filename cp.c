#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        printf("Warning !! You must input 2 file name!\n");
    }
    else 
    {
        char tt[256] = {0};
        snprintf(tt, sizeof(tt), "cp %s %s ", argv[1], argv[2]);
        int ret = system(tt);
        printf("ret = %d\n", ret);
    }
    return 0;
}
