#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <crypt.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <uname>\n", argv[0]);
        exit(1);
    }
    const char *test_pwd = getpass("Your password: ");
    if (test_pwd == NULL)
    {
        perror("getpass() ");
        exit(2);
    }
    struct spwd *shadow_line = getspnam(argv[1]);
    if (shadow_line == NULL)
    {
        fprintf(stderr, "getspnam() failed\n");
        exit(3);
    }
    const char *crypt_pwd = crypt(test_pwd, shadow_line->sp_pwdp);
    if (strcmp(crypt_pwd, shadow_line->sp_pwdp) == 0)
        puts("ok");
    else
        puts("fail");
    exit(0);
}