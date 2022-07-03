#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *line_buf = NULL;
    size_t line_num = 0;
    getline(&line_buf, &line_num, stdin);
    printf("%s", line_buf);
    free(line_buf);
    // line_buf = NULL;
    getline(&line_buf, &line_num, stdin);
    printf("%s", line_buf);
    free(line_buf);
    exit(EXIT_SUCCESS);
}
