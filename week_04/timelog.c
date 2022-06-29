#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FILE_NAME   "/tmp/out"
#define BUF_SIZE    1024

int main(int argc, char const *argv[])
{
    FILE *file = fopen(FILE_NAME, "a+");
    if (file == NULL)
    {
        perror("can't open ");
        exit(1);
    }
    time_t tstamp;
    struct tm *my_time = NULL;
    int cnt = 0;
    char buf_line[BUF_SIZE];
    while (fgets(buf_line, BUF_SIZE, file) != NULL)
        cnt++;
    while (1)
    {
        time(&tstamp);
        my_time = localtime(&tstamp);
        fprintf(file, "%d. %d %d %d %d-%d-%d\n", ++cnt, my_time->tm_year+1900, 
        my_time->tm_mon+1, my_time->tm_mday, my_time->tm_hour, 
        my_time->tm_min, my_time->tm_sec);
        fflush(file);
        sleep(1);
    }
    fclose(file);
    exit(0);
}
