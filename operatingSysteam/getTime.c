/*************************************************************************
	> File Name: testClock.c
	> Author: 
	> Mail: 
	> Created Time: Thu 08 Oct 2015 03:32:36 PM
 ************************************************************************/

#include<time.h>
#include"getTime.h"
long getCurrentTime()
{
    clock_t ms = clock();
    time_t rawtime;
    time(&rawtime);
    long startTime = rawtime*1000+ms;
    return startTime;
}
void delay(unsigned long msec)
{
    clock_t start;
    start = clock();
    while(clock()-start < msec);
}
