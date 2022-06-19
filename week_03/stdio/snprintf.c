#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN    1024

int main(int argc, char const *argv[])
{
    int year = 2022, month = 6, day = 15;
    char date[BUF_LEN];
    snprintf(date, 4, "%d-%d-%d", year, month, day);
    printf("%s\n", date);
    exit(0);
}
