#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN    20

int main(int argc, char const *argv[])
{
    char line_buf[BUF_LEN];
    char *p = NULL;
    FILE *file = fopen("test11.dat", "r");
    if (file == NULL)
    {
        perror("can't open");
        exit(-1);
    }

    while (fgets(line_buf, BUF_LEN, file) != NULL)
    {
        if ((p = strchr(line_buf, '\n')) != NULL)
        {
            *p = '\0';
        }
        printf("%s ", line_buf);
    }

    fclose(file);

    exit(0);
}
