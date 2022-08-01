#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include "protocol.h"

#define BUF_SIZE 40
#define IP_SIZE 20

int main(int argc, char const *argv[])
{
    int sock_fd, new_fd;
    struct sockaddr_in laddr, raddr;
    socklen_t addr_len;
    char ipstr[IP_SIZE];
    int flag;
    pid_t pid;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    flag = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) < 0)
    {
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }

    memset(&laddr, 0, sizeof(laddr));
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(PORT));
    if (inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if (bind(sock_fd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, CONN_NUM) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        addr_len = sizeof(raddr);
        if ((new_fd = accept(sock_fd, (void *)&raddr, &addr_len)) < 0)
        {
            perror("accept()");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            close(sock_fd);
            memset(ipstr, 0, IP_SIZE);
            if (inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr, IP_SIZE) < 0)
            {
                perror("inet_ntop()");
                exit(EXIT_FAILURE);
            }
            printf("---MESSAGE FROM---%s:%d\n", ipstr, ntohs(raddr.sin_port));

            char buf[BUF_SIZE];
            memset(buf, 0, BUF_SIZE);
            int len = sprintf(buf, STAMP_FMT, (long long)time(NULL));

            if (send(new_fd, buf, len, 0) < 0)
            {
                perror("send()");
                exit(EXIT_FAILURE);
            }
            close(new_fd);
            exit(EXIT_SUCCESS);
        }

        close(new_fd);
    }

    close(sock_fd);
    exit(EXIT_SUCCESS);
}