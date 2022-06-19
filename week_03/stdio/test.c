#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN    10

int main(int argc, char const *argv[])
{
    char line_buffer[LEN];
    FILE *data = fopen("data", "r+");
    if (data == NULL)
    {
        fprintf(stderr, "can't open: %s", strerror(errno));
        exit(1);
    }
    char ch = fgetc(data);
    printf("%c\n", ch);
    fputc('A', data);
    ch = fgetc(data);
    printf("%c\n", ch);
    fclose(data);
    exit(0);
}
