#include <stdio.h>
#include <string.h>

int main()
{
	char *res;
	
	char src[] = "abc/def/ghi";
    char aaa[13] = {0};
    strcpy(aaa,src);
	char *p = aaa;

	while( res = strsep(&p, "/"))
    {
        printf( "res = %s\n", res);
    }

	//printf("%s\n", res);
	//printf("p = %s\n", p);
	return 0;
}

