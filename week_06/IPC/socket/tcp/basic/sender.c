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
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd;
    struct sockaddr_in raddr;
    FILE *file = NULL;
    long long stamp;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    memset(&raddr, 0, sizeof(raddr));
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &raddr.sin_addr.s_addr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if (connect(sock_fd, (void *) &raddr, sizeof(raddr)) < 0)
    {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

    file = fdopen(sock_fd, "r+");
    
    fscanf(file, STAMP_FMT, &stamp);
    printf("%lld\n", stamp);

    close(sock_fd);
    exit(EXIT_SUCCESS);
}
