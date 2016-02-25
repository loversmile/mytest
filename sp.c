#include <stdio.h>

int main(int argc, char *argv[])
{
	char buf[256][256];
	int i = 0, j = 0;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			sprintf(buf[i * 10 + j], "[%d]",i * 10 + j);
		}
	}
	for(i = 0; i < 100; i++)
	{
		printf("%s", buf[i]);
		if(i % 9 == 0 && i != 0)
			printf("\n");
	}
	return 0;
}
