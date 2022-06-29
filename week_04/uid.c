#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <uid>\n", argv[0]);
        exit(1);
    }
    struct passwd *my_pwd = getpwuid(atoi(argv[1]));
    if (my_pwd != NULL)
        puts(my_pwd->pw_name);
    exit(0);
}
