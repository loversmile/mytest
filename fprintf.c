#include <stdio.h>

int main()
{
	FILE * fp = fopen("fp.txt", "w");
	fprintf(fp, "first...");
	fprintf(fp, "second...");
	fprintf(fp, "third...");
	fprintf(fp, "forth...\n");
	fclose(fp);
	return 0;
}
