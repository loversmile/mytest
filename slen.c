#include <stdio.h>
#include <string.h>

int main()
{
	char *temp = "5678";
	char *p = NULL;
	p = strdup(temp);
	printf("p = %s\n", p);
	char a = 0;
	while((a=*p) == ' ')
	{
		p++;
	}
	printf("a = %c\n", a);
}
