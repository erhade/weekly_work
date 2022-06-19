#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN    1024    

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <src> <dst>\n", argv[0]);
        exit(1);
    }
    FILE *src = fopen(argv[1], "r");
    if (src == NULL)
    {
        perror("can't open src ");
        exit(2);
    }
    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL)
    {
        perror("can't open dst ");
        exit(2);
    }
    int n;
    char line_buf[BUF_LEN];
    while ((n = fread(line_buf, 1, BUF_LEN, src)))
        fwrite(line_buf, 1, n, dst);
    fclose(src);
    fclose(dst);
    exit(0);
}
