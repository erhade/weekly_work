#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>

#include "protocol.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) >= NAME_MAX)
    {
        fprintf(stderr, "The length of name is out of size.\n");
        exit(EXIT_FAILURE);
    }

    int sock_fd = socket(DOMAIN, TYPE, 0);
    if (sock_fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    struct ip_mreqn mreqn;
    memset(&mreqn, 0, sizeof(mreqn));
    if (inet_pton(AF_INET, GROUP, &mreqn.imr_multiaddr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if (inet_pton(AF_INET, "0.0.0.0", &mreqn.imr_address) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if ((mreqn.imr_ifindex = if_nametoindex("eth0")) == 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn)) < 0)
    {
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }

    uint64_t size = sizeof(msg_t) + strlen(argv[1]);
    msg_t *msg_buf = malloc(size);
    if (msg_buf == NULL)
    {
        fprintf(stderr, "Malloc fail.\n");
        exit(EXIT_FAILURE);
    }
    /* 发送数据之前一定做memset 不然会造成栈内存内容泄露到发的包里 */
    memset(msg_buf, 0, size);
    strcpy((char *)msg_buf->name, argv[1]);
    msg_buf->math = htonl(95);
    msg_buf->chinese = htonl(87);

    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = DOMAIN;
    if (inet_pton(DOMAIN, GROUP, &s_addr.sin_addr.s_addr) < 0)
    {
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    s_addr.sin_port = htons(atoi(PORT));

    if (sendto(sock_fd, msg_buf, size, 0, (void *) &s_addr, (socklen_t) sizeof(s_addr)) < 0)
    {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }

    free(msg_buf);

    close(sock_fd);
    exit(EXIT_SUCCESS);
}
