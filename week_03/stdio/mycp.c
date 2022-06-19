#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <src_dir> <dst_dir>\n", argv[0]);
        exit(1);
    }
    const char *src_dir = argv[1];
    const char *dst_dir = argv[2];
    FILE *src_file = NULL;
    FILE *dst_file = NULL;
    char ch;
    src_file = fopen(src_dir, "r");
    if (src_file == NULL)
    {
        perror("can't open src_file ");
        exit(2);
    }
    dst_file = fopen(dst_dir, "w");
    if (dst_file == NULL)
    {
        fclose(src_file);
        perror("can't open dst_file ");
        exit(2);
    }
    while ((ch = fgetc(src_file)) != EOF)
        fputc(ch, dst_file);
    fclose(src_file);
    fclose(dst_file);
    exit(0);
}