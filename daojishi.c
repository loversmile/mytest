/*************************************************************************
	> File Name: daojishi.c
	> Author: 
	> Mail: 
	> Created Time: 2015年03月17日 星期二 09时06分14秒
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct ymd{
    int year;
    int month;
    int day;
    struct ymd *next;
};

int hasR(int y, int yy);
int toEnd(int y, int m, int d);
int toStart(int y, int m, int d);
int isR(int year);
int func(struct ymd *nowday, struct ymd *setday);


int md[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


int main(int argc, char *argv[])
{
    time_t nowtime;
    struct tm *timeinfo;
    time(&nowtime);
    timeinfo = localtime(&nowtime);
    int year, month, day;
    year = timeinfo->tm_year + 1900;
    month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;
    struct ymd *nowday = (struct ymd*)malloc(sizeof(struct ymd));
    struct ymd *setday = (struct ymd*)malloc(sizeof(struct ymd));
    nowday->year = year;
    nowday->month = month;
    nowday->day = day;
    printf("%d-%d-%d\n", year, month, day);
    //int yy, mm, dd;
    //scanf("%d,%d,%d", &yy, &mm, &dd);
    scanf("%d,%d,%d", &setday->year, &setday->month, &setday->day);
    printf("%d,%d,%d\n", setday->year, setday->month, setday->day);
    //printf("%d-%d-%d\n", yy, mm, dd);
    int res = func(nowday, setday);
    printf("The lovely day is %d\n", res);
    return 0;
}

/* nowdat, setday */
int func(struct ymd *nowday, struct ymd *setday)
{
    int y = nowday->year;
    int m = nowday->month;
    int d = nowday->day;
    int yy = setday->year;
    int mm = setday->month;
    int dd = setday->day;
    int returnday = 0;
    if( (y > yy) || (y == yy && m > mm) || (y == yy && m == mm && d > dd) )
    {
        if (y > yy)
            returnday = hasR(y, yy) + 365 *(y - yy -1) + toEnd(yy, mm, dd) + toStart(y, m, d);
        else if (y == yy)
            returnday = toEnd(yy, mm, dd) - toEnd(y, m, d);
    } 
    else if ( y == yy && m == mm && d == dd )
    {
        return 0;
    }
    else
    {
        if (yy > y)
            returnday = hasR(yy, y) + 365 *(yy - y -1) + toEnd(y ,m, d) + toStart(yy, mm, dd);
        else if (y == yy)
            returnday = toEnd(y, m, d) - toEnd(yy, mm, dd);
    }
    return returnday;
}

/* y > yy */
int hasR(int y, int yy)
{
    printf("y = %d, yy = %d\n", y, yy);
    int i = 0;
    int tmp = 0;
    for ( i = yy + 1; i < y; ++i )
    {
        if (isR(i))
            tmp += 1;
    }
    printf("hasR %d\n", tmp);
    return tmp;
}

/* Judge is Leep year */
int isR(int year)
{
    if( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        return 1;
    return 0;
}

/* Not include the day*/
int toEnd(int y, int m, int d)
{
    if (isR(y))
    {
        printf( "toEnd %d\n", (366 - toStart(y, m, d)));
        return (366 - toStart(y, m, d));
    }
    printf( "toEnd %d\n", (365 - toStart(y, m, d)));
    return (365 - toStart(y, m, d));
}

/*Include the day*/
int toStart(int y, int m, int d)
{
    int i = 0;
    int total = 0;
    for (i = 1; i < m; ++i)
    {
        total += md[i];
    }
    if (isR(y) && m > 2)
    {
        printf("toStart %d\n", (total + d + 1) );
        return (total + d + 1);
    }
        printf("toStart %d\n", (total + d ) );
    return (total + d);
}
