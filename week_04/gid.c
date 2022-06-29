#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <gid>\n", argv[0]);
        exit(1);
    }
    struct group *my_grp = getgrgid(atoi(argv[1]));
    if (my_grp)
    {
        int i = 0;
        printf("gid: %u, gnam: %s, passwd: %s\n", my_grp->gr_gid, 
        my_grp->gr_name, my_grp->gr_passwd);
        while (my_grp->gr_mem[i])
            puts(my_grp->gr_mem[i++]);
    }
    exit(0);
}
