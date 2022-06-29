#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIME_SIZE   1024

int main(int argc, char const *argv[])
{
    time_t time_stamp;
    struct tm *local_time = NULL;
    char f_time[TIME_SIZE];
    time_stamp = time(NULL);
    local_time = localtime(&time_stamp);
    strftime(f_time, TIME_SIZE, "%Y-%m-%d", local_time);
    puts(f_time);
    local_time->tm_mday += 100;
    (void)mktime(local_time);
    strftime(f_time,TIME_SIZE, "%Y-%m-%d", local_time);
    puts(f_time);
    exit(0);
}
