#include <stdio.h>

int main()
{
    float a = 13.2;
    printf("%0.3f\n", a);

    float b = 0.0;
    char* ss = "13.2";
    sscanf(ss, "%0.3f", &b);
    printf("%f\n", b);
    return 0;
}
