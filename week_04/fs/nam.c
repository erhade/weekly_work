#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        exit(1);
    }
    struct passwd *my_pwd = getpwnam(argv[1]);
    if (my_pwd)
        printf("%u\n", my_pwd->pw_uid);
    exit(0);
}
