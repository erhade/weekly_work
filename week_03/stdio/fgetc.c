#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <src_dir>\n", argv[0]);
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    int count = 0;
    if (file == NULL)
    {
        perror("can't open file ");
        exit(2);
    }
    while (fgetc(file) != EOF)
        count++;
    printf("%d\n", count);
    fclose(file);
    exit(0);
}
