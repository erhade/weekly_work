#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>

#include "protocol.h"

#define IP_SIZE    20

int main(int argc, char const *argv[])
{
    struct sockaddr_in s_addr, c_addr;
    socklen_t socklen;
    msg_t *msg_buf = NULL;
    char ipstr[IP_SIZE];

    int fd = socket(DOMAIN, TYPE, 0);
    if (fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    s_addr.sin_family = DOMAIN;
    if (inet_pton(DOMAIN, ADDR, &s_addr.sin_addr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    s_addr.sin_port = htons(atoi(PORT));

    if (bind(fd, (void *)&s_addr, sizeof(s_addr)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    uint64_t size = sizeof(msg_t) + NAME_MAX - 1;
    msg_buf = malloc(size);
    if (msg_buf == NULL)
    {
        fprintf(stderr, "Malloc fail.\n");
        exit(EXIT_FAILURE);
    }

    /* !!! */
    socklen = sizeof(c_addr);

    while (recvfrom(fd, msg_buf, size, 0, (void *) &c_addr, &socklen) >= 0)
    {
        if (inet_ntop(DOMAIN, &c_addr.sin_addr, ipstr, (socklen_t) IP_SIZE) < 0)
        {
            perror("inet_ntop()");
            exit(EXIT_FAILURE);
        }
        printf("---MESSAGE FROM %s:%u---\n", ipstr, ntohs(c_addr.sin_port));
        printf("name: %s\n", msg_buf->name);
        printf("math: %u\n", ntohl(msg_buf->math));
        printf("chinese: %u\n", ntohl(msg_buf->chinese));
    }

    perror("recvform()");
    close(fd);
    free(msg_buf);
    exit(EXIT_FAILURE);
}
