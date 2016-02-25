#include <string.h>
#include <stdio.h>

int main()
{
    char *category = "(]1[)";
    const char *start = strstr(category, "(]");
    const char *end = strstr(category, "[)");

    printf("%s\n", start);
    printf("%s\n", end);
    return 0;
}
