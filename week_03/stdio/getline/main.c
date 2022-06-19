#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file_dir>\n", argv[0]);
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("can't open file: ");
        exit(2);
    }
    char *line = NULL;
    size_t line_num = 0;
    while (getline(&line, &line_num, file) >= 0)
    {
        printf("%zu\n", strlen(line));
        free(line);
        line = NULL;
    }

    exit(0);
}
