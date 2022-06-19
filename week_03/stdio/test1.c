#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN    10

int main(int argc, char const *argv[])
{
    FILE *file = fopen("data1", "w");
    if (file == NULL)
    {
        // perror("cant't open ");
        fprintf(stderr, "can't open: %s\n", strerror(errno));
        exit(1);
    }
    char line_buffer[LEN];
    while (fgets(line_buffer, LEN-1, file))
    {
        line_buffer[LEN-1] = '\0';
        printf("%s", line_buffer);
    }
    fclose(file);
    exit(0);
} 