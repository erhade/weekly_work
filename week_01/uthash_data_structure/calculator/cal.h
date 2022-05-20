#ifndef CAL_H__
#define CAL_H__

typedef struct cal_st
{
    const char *exp;
    int result;
}cal_st;

cal_st *cal_new(const char *exp);

int cal_process(cal_st *cal);

void cal_free(cal_st **cal);


#endif