#ifndef RELAYER_H__
#define RELAYER_H__

#define JOB_SIZE    1024

char *relayer_err_str(int err_no);

int relayer_init(int fd_src, int fd_dst);

int relayer_destroy(int rl_id);

#endif