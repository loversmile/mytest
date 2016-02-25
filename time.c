#include <stdio.h>
#include <time.h>

int main()
{
	printf("%s\n", __TIME__);
    printf("%d\n", time(NULL));
	return 0;
}
