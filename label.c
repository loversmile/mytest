#include <stdio.h>

int main()
{
	int a = 1;
	if(1 != a )
	{
		printf("IF DEG\n");
		goto out;
		printf("IF DEG 222\n");
	}
out:
	printf("This is out label!\n");
	return 0;
}
