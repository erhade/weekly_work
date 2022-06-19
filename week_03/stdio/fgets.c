#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN    5

int main(int argc, char const *argv[])
{
    char line_buf[BUF_LEN];
    fgets(line_buf, BUF_LEN, stdin);
    printf("%s\n", line_buf);
    fputs("AB\n", stdout);
    exit(0);
}
