#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char * buff;
    buff[0] = '\0';
    printf("strleni  \n", strlen(buff));
    strcat(buff, "asdhfsd");
    printf("%s\n", buff);
}
