/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2014年10月08日 星期三 11时49分40秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>

int main(){

    int ret = system("/app/asterisk/bin/rsync /app/asterisk/var/spool/asterisk/monitor_local/ /media/sdb1/PBX_Recordings_000b824d3aee -rut");
    printf("ret = %d\n", ret);
    return 0;
}
