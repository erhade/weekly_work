#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "protocol.h"

int main(int argc, char const *argv[])
{
    int msg_id;
    key_t key;
    msg_st msg_buf;

    if ((key = ftok(KEY_PATHNAME, KEY_PROJID)) < 0)
    {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }

    if ((msg_id = msgget(key, 0)) < 0)
    {
        perror("msgget()");
        exit(EXIT_FAILURE);
    }

    msg_buf.mtype = 1;
    strncpy(msg_buf.name, "Alice", NAME_SIZE);
    msg_buf.chinese = 89;
    msg_buf.math = 95;

    if (msgsnd(msg_id, &msg_buf, sizeof(msg_buf) - sizeof(long), 0) < 0)
    {
        perror("msgsnd()");
        exit(EXIT_FAILURE);
    }
    puts("send one msg...");

    exit(EXIT_SUCCESS);
}
