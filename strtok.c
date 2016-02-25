#include <string.h>
#include <stdio.h>
int main(void)
{
    char input[64] = "{\"response\" = {\"chanllege\":\"asdjofhsdajkfh\"}}";
    char *p = NULL;
    char *dd = NULL;
    /**/ /* strtok places a NULL terminator
    in front of the token, if found */
    p = strtok(input, "\"");
    printf("p = %s\n", p);
    while(p != NULL)
    {
        dd = p;
        printf("dd = %s\n", dd);
        if(dd)
        {
            dd = NULL;
        }
        p = strtok(NULL, "\"");
    }
    return 0;
}
