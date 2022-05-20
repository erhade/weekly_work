#include <stdio.h>
#include <stdlib.h>

#include "cal.h"

int main(int argc, char const *argv[])
{
    cal_st *my_cal = cal_new("33 + 45 * 3 - (6 * (5 + 6) / 3) * 2");
    cal_process(my_cal);
    printf("%s = %d\n", my_cal->exp, my_cal->result);
    cal_free(&my_cal);
    exit(0);
}
