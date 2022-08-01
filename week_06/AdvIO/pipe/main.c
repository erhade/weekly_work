#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mypipe.h"

#define BUF_SIZE 1024

int main(int argc, char const *argv[])
{
    char buf[BUFSIZ];
    char dst[BUFSIZ];
    mypipe_t *mypipe = mypipe_init();
    mypipe_register(mypipe, PIPE_READ | PIPE_WRITE);

    strcpy(buf, "hello world\n");

    mypipe_write(mypipe, buf, 13);

    mypipe_read(mypipe, dst, 13);

    puts(dst);

    mypipe_unregister(mypipe, PIPE_READ | PIPE_WRITE);

    mypipe_destroy(mypipe);

    exit(EXIT_SUCCESS);
}
