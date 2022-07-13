#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#define JOB_MAX    1024
typedef void (*JOB)(const char *str);

int anytimer_init(int sec, JOB func, char *msg, bool is_repeat);

int anytimer_cancel(int job_id);

int anytimer_wait(int job_id);

int anytimer_stop(int job_id);

int anytimer_resume(int job_id);

#endif