#include <stdio.h>
#include <string.h>

int main()
{
	char *s = "SIP/5000-00000001";
	char a[8] = "";
	char p[80];
//	char *q = a;
//	char *t = q;
	strcpy(p, s);
	int i = 0;
	int j = 0;
	while(p[j] && p[j] != '-'){
		if(p[j] > '9' || p[j] < '0')
		{
			j++;
		}else{
			a[i]= p[j];
			j++;
			i++;
		}
	}
	printf("%s\n", a);
	return 0;
}
