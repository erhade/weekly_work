#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define TIME_SIZE   1024

/* 
 * -Y year
 * -m month
 * -d day
 * -H hour
 * -M minute
 * -S second
 */

int main(int argc, char const *argv[])
{
    char time_buf[TIME_SIZE];
    char format[TIME_SIZE];
    memset(format, 0, TIME_SIZE);
    int short_opt;
    FILE *file_out = stdout;
    time_t time_stamp = time(NULL);
    struct tm *l_time = localtime(&time_stamp);
    while (1)
    {
        short_opt = getopt(argc, (char *const *)argv, "-Y:mdH:MS");
        if (short_opt < 0)
            break;
        if (short_opt == '?')
            exit(EXIT_FAILURE);
        if (short_opt == 1)
        {
            if (file_out == stdout)
            {
                file_out = fopen(argv[optind-1], "w");
                if (file_out == NULL)
                {
                    perror("can't open ");
                    file_out = stdout;
                }
            }
            continue;
        }
        switch (short_opt)
        {
        case 'Y':
            if (strcmp(optarg, "2") == 0)
                strncat(format, "%y ", TIME_SIZE - strlen(format) - 1);
            else if (strcmp(optarg, "4") == 0)
                strncat(format, "%Y ", TIME_SIZE - strlen(format) - 1);
            else
            {
                fprintf(stderr, "-%c Invalid argument %s\n", short_opt, optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 'm':
            strncat(format, "%m ", TIME_SIZE - strlen(format) - 1);
            break;
        case 'd':
            strncat(format, "%d ", TIME_SIZE - strlen(format) - 1);
            break;
        case 'H':
            if (strcmp(optarg, "12") == 0)
                strncat(format, "%I(%P) ", TIME_SIZE - strlen(format) - 1);
            else if (strcmp(optarg, "24") == 0)
                strncat(format, "%H ", TIME_SIZE - strlen(format) - 1);
            else 
            {
                fprintf(stderr, "-%c Invalid argument %s\n", short_opt, optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 'M':
            strncat(format, "%M ", TIME_SIZE - strlen(format) - 1);
            break;
        case 'S':
            strncat(format, "%S ", TIME_SIZE - strlen(format) - 1);
            break;
        default:
            _exit(EXIT_FAILURE);
            break;
        }
    }
    
    format[TIME_SIZE-1] = '\0';
    strftime(time_buf, TIME_SIZE, format, l_time);
    strncat(time_buf, "\n", TIME_SIZE - strlen(time_buf) - 1);
    time_buf[TIME_SIZE-1] = '\0';
    fputs(time_buf, file_out);
    if (file_out != stdout)
        fclose(file_out);
    exit(EXIT_SUCCESS);
}
