#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
//	printf("%s\n", __TIME__);
 //   printf("%d\n", time(NULL));
    time_t t;
//    t = time(&t);

    putenv("TZ=EST-8EDT");
    tzset();
    struct tm *tblock;  
    t=time(NULL);  
    tblock=localtime(&t);  
    printf("Local time is: %s",asctime(tblock));  
    printf("%04d%02d%02d%02d%02d%02d\n", tblock->tm_year+1990, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
	return 0;
}
