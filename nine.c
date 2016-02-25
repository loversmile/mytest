#include <stdio.h>

int main(int argc, char * argv[])
{
	int i = 0, j = 0;
	for(i = 1; i < 10; i++)
	{
		for(j = 1; j <= i; j++)
		{
			printf("%d*%d=%d\t", j, i, j * i);
		}
		printf("\n");
	}
	return 0;
}

