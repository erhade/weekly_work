#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define NAME_SIZE   512
#define TIME_SIZE   512

static void get_file_names(const char *pattern, int flags, glob_t *my_glob)
{
    int ret;
    if ((ret = glob(pattern, flags, NULL, my_glob)) != 0)
    {
        fprintf(stderr, "Error No is %d", ret);
        exit(EXIT_FAILURE);
    }
}

static char *get_mode(struct stat *stat_buf)
{
    char *ret_mode = malloc(sizeof(char) * 11);
    if (ret_mode == NULL)
    {
        fprintf(stderr, "Malloc Failed\n");
        exit(EXIT_FAILURE);
    }
    mode_t mode = stat_buf->st_mode;
    switch (mode & S_IFMT)
    {
    case S_IFSOCK:
        ret_mode[0] = 's';
        break;
    case S_IFLNK:
        ret_mode[0] = 'l';
        break;
    case S_IFREG:
        ret_mode[0] = '-';
        break;
    case S_IFBLK:
        ret_mode[0] = 'b';
        break;
    case S_IFCHR:
        ret_mode[0] = 'c';
        break;
    case S_IFDIR:
        ret_mode[0] = 'd';
        break;
    case S_IFIFO:
        ret_mode[0] = 'p';
        break;
    default:
        _exit(EXIT_FAILURE);
        break;
    }
    ret_mode[1] = mode & S_IRUSR ? 'r' : '-';
    ret_mode[2] = mode & S_IWUSR ? 'w' : '-';
    if (mode & S_IXUSR)
    {
        if (mode & S_ISUID)
            ret_mode[3] = 's';
        else
            ret_mode[3] = 'x';
    }
    else
    {
        if (mode & S_ISUID)
            ret_mode[3] = 'S';
        else
            ret_mode[3] = '-';
    }
    ret_mode[4] = mode & S_IRGRP ? 'r' : '-';
    ret_mode[5] = mode & S_IWGRP ? 'w' : '-';
    if (mode & S_IXGRP)
    {
        if (mode & S_ISGID)
            ret_mode[6] = 's';
        else
            ret_mode[6] = 'x';
    }
    else
    {
        if (mode & S_ISGID)
            ret_mode[6] = 'S';
        else
            ret_mode[6] = '-';
    }
    ret_mode[7] = mode & S_IROTH ? 'r' : '-';
    ret_mode[8] = mode & S_IWOTH ? 'w' : '-';
    if (mode & S_IXOTH)
    {
        if (mode & S_ISVTX)
            ret_mode[9] = 't';
        else
            ret_mode[9] = 'x';
    }
    else
    {
        if (mode & S_ISVTX)
            ret_mode[9] = 'T';
        else
            ret_mode[9] = '-';
    }
    ret_mode[10] = '\0';
    return ret_mode;
}

static void show_stats(struct stat *stats, bool i_flag, bool is_id)
{
    char *ret_mode = NULL;
    char time_buf[TIME_SIZE];
    struct passwd *pw = getpwuid(stats->st_uid);
    struct group *gr = getgrgid(stats->st_gid);
    if (i_flag == true)
        printf("%lu ", stats->st_ino);
    ret_mode = get_mode(stats);
    printf("%s ", ret_mode);
    free(ret_mode);
    printf("%4lu ", stats->st_nlink);
    if (is_id == true)
    {
        printf("%u ", stats->st_uid);
        printf("%u ", stats->st_gid);
    }
    else
    {
        printf("%s ", pw->pw_name);
        printf("%s ", gr->gr_name);
    }
    printf("%6ld ", stats->st_size);
    struct tm *l_time = localtime(&(stats->st_mtim.tv_sec));
    strftime(time_buf, TIME_SIZE, "%b %d %H:%M ", l_time);
    printf("%s ", time_buf);
}

static void show_names(glob_t *my_glob, char i_flag, char l_flag, char n_flag)
{
    struct stat *stat_arr = malloc(sizeof(*stat_arr) * my_glob->gl_pathc);
    if (stat_arr == NULL)
    {
        fprintf(stderr, "Malloc Failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < my_glob->gl_pathc; i++)
    {
        if (lstat(my_glob->gl_pathv[i], stat_arr + i) < 0)
        {
            perror("stat() ");
            exit(EXIT_FAILURE);
        }
    }
    if (n_flag == true || l_flag == true)
    {
        for (int i = 0; i < my_glob->gl_pathc; i++)
        {
            show_stats(stat_arr + i, i_flag, n_flag);
            printf("%s\n", strrchr(my_glob->gl_pathv[i], '/') + 1);    
        }
    }
    else
    {
        for (int i = 0; i < my_glob->gl_pathc; i++)
        {
            if (i_flag == true)
                printf("%lu ", stat_arr[i].st_ino);
            printf("%s ", strrchr(my_glob->gl_pathv[i], '/') + 1);
        }
        putchar('\n');
    }
    free(stat_arr);
}

int main(int argc, char const *argv[])
{
    char opt;
    char l_flag = false, i_flag = false, a_flag = false, n_flag = false, 
    not_opt_falg = false;
    int cnt = 0;
    glob_t my_glob;
    char opt_arr[argc];
    memset(opt_arr, false, argc);
    char file_name[NAME_SIZE];
    while (true)
    {
        opt = getopt(argc, (char *const *)argv, "-lian");
        if (opt == -1)
            break;
        if (opt == '?')
            exit(EXIT_FAILURE);
        if (opt == 'l')
            l_flag = true;
        else if (opt == 'i')
            i_flag = true;
        else if (opt == 'a')
            a_flag = true;
        else if (opt == 'n')
            n_flag = true;
        else if (opt == 1)
        {
            opt_arr[optind-1] = true;
            not_opt_falg = true;
            cnt++;
        }
        else
            _exit(EXIT_FAILURE);
    }
    if (not_opt_falg == false)
    {
        if (a_flag == true)
        {
            get_file_names("./.*", 0, &my_glob);
            get_file_names("./*", GLOB_APPEND, &my_glob);
        }
        else
            get_file_names("./*", 0, &my_glob);       
        show_names(&my_glob, i_flag, l_flag, n_flag);
        globfree(&my_glob);
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            if (opt_arr[i] == true)
            {
                if (cnt != 1)
                    printf("%s:\n", argv[i]);
                if (a_flag == true)
                {
                    strncpy(file_name, argv[i], NAME_SIZE - 1);
                    strncat(file_name, "/.*", NAME_SIZE - strlen(file_name) - 1);
                    file_name[NAME_SIZE-1] = '\0';
                    get_file_names(file_name, 0, &my_glob);
                    strncpy(file_name, argv[i], NAME_SIZE - 1);
                    strncat(file_name, "/*", NAME_SIZE - strlen(file_name) - 1);
                    file_name[NAME_SIZE-1] = '\0';
                    get_file_names(file_name, GLOB_APPEND, &my_glob);
                }
                else
                {
                    strncpy(file_name, argv[i], NAME_SIZE - 1);
                    strncat(file_name, "/*", NAME_SIZE - strlen(file_name) - 1);
                    file_name[NAME_SIZE-1] = '\0';
                    get_file_names(file_name, 0, &my_glob);
                }
                show_names(&my_glob, i_flag, l_flag, n_flag);
                globfree(&my_glob);
            }
        }
        
    }

    exit(EXIT_SUCCESS);
}