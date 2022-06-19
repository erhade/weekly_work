#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN    10

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <src_dir> <dst_dir>\n", argv[0]);
        exit(1);
    }
    FILE *src = fopen(argv[1], "r");
    if (src == NULL)
    {
        perror("can't open src_file ");
        exit(2);
    }
    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL)
    {
        fclose(src);
        perror("can't open dst_file ");
        exit(2);
    }
    char line_buf[BUF_LEN];
    while (fgets(line_buf, BUF_LEN, src) != NULL)
        fputs(line_buf, dst);
    fclose(src);
    fclose(dst);
    exit(0);
}
