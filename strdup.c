#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char *s = "This is a genius wrote.";
	char *d;
	system("clear");
	d = strdup(s);
	printf("the lenth of s is %d\n", strlen(s));
	printf("the lenth of d is %d\n", strlen(d));
	printf("%s\n", d);
	free(d);
//	getchar();
	return 0;
}
