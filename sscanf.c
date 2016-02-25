#include <string.h>
#include <stdio.h>

int main()
{
	char re[512] = {0};
	char* aa = "abcdefgrt";

	sscanf(aa, "%s", re);

	printf("re = %s\n", re);
    
    int bc = 0;
    char *M = "MaxBR=1280";
    int pp = sscanf(M, "MaxBR=%30u", &bc);
    printf("pp = %d\n", pp);
    printf("M=%u", bc);

	return 0;
}
