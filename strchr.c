#include <stdio.h>
#include <string.h>

int main()
{
	char str_a[] = "abc/def/ghi";

	char* b = strchr(str_a, '/');

	printf("%s\n", b);

	return 0;
}
