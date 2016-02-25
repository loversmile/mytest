#include <string.h>
#include <stdio.h>

int main()
{
    const char *key = NULL;
    const char *key2 = NULL;

    char aa[80] = "a,b,c,d,e,f";

    char *p = aa;
    printf("%s\n", p);
    key = strtok(aa,",");
    printf("%s\n", key);
    printf("%s\n", aa);
    printf("%s\n", p);
    key2 = strtok(NULL,",");
    printf("%s\n", key2);
}
