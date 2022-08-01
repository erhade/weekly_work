#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#include "protocol.h"

static int msg_id; 

static void sigint_handler(int s)
{
    if (msgctl(msg_id, IPC_RMID, NULL) < 0)
    {
        perror("msgctl()");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    key_t key;
    msg_st msg_buf;
    struct sigaction act;

    act.sa_handler = sigint_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGTERM);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);

    if ((key = ftok(KEY_PATHNAME, KEY_PROJID)) < 0)
    {
        perror("ftok()");
        exit(EXIT_FAILURE);
    }

    if ((msg_id = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget()");
        exit(EXIT_SUCCESS);
    }
    
    while(msgrcv(msg_id, &msg_buf, sizeof(msg_buf)- sizeof(long), 0, 0) != -1)
    {
        printf("name: %s\n", msg_buf.name);
        printf("math: %d\n", msg_buf.math);
        printf("chinese: %d\n", msg_buf.chinese);
    }

    exit(EXIT_SUCCESS);
}
