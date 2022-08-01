#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_MAX        1024
#define PHOTO_NAME    "lena.png"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd;
    struct sockaddr_in raddr;
    FILE *file = NULL;
    FILE *file_tmp = NULL;
    char buf[BUF_MAX];
    int len;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    memset(&raddr, 0, sizeof(raddr));
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(80);
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
    if (file == NULL)
    {
        perror("fdopen()");
        exit(EXIT_FAILURE);
    }
    file_tmp = fopen(PHOTO_NAME, "w");
    if (file_tmp == NULL)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "GET /%s\r\n\r\n", PHOTO_NAME);
    fflush(file);

    while (1)
    {
        len = fread(buf, 1, BUF_MAX, file);
        if (len <= 0)
            break;
        fwrite(buf, 1, len, file_tmp);
    }

    fclose(file);
    fclose(file_tmp);
    exit(EXIT_SUCCESS);
}
