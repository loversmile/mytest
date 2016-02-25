
#include <stdio.h>

int main()
{
	char buf[512] = {0};
	FILE* fp = NULL;
	fp = fopen("aa.txt", "r");
	while(fgets(buf, sizeof(buf), fp))
	{
		puts(buf);
	}

	



	return 0;
}
