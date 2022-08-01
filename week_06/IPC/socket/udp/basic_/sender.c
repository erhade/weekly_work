#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "protocol.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <ip_addr>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd = socket(DOMAIN, TYPE, 0);
    if (sock_fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    msg_t msg_buf;
    memset(&msg_buf, 0, sizeof(msg_buf));
    strcpy((char *)msg_buf.name, "Alice");
    msg_buf.math = htonl(95);
    msg_buf.chinese = htonl(87);

    struct sockaddr_in s_addr;
    s_addr.sin_family = DOMAIN;
    if (inet_pton(DOMAIN, argv[1], &s_addr.sin_addr.s_addr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    s_addr.sin_port = htons(atoi(PORT));

    if (sendto(sock_fd, &msg_buf, sizeof(msg_buf), 0, (void *) &s_addr, (socklen_t) sizeof(s_addr)) < 0)
    {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }

    close(sock_fd);
    exit(EXIT_SUCCESS);
}
