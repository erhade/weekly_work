#ifndef MYPIPE_H__
#define MYPIPE_H__

#define PIPE_SIZE       1024
#define PIPE_READ       0x00000001
#define PIPE_WRITE      0x00000002

typedef void mypipe_t;

mypipe_t *mypipe_init(void);

int mypipe_register(mypipe_t *pipe, int flags);

int mypipe_unregister(mypipe_t *pipe, int flags);

int mypipe_read(mypipe_t *pipe, char *buf, int len);

int mypipe_write(mypipe_t *pipe, const char *buf, int len);

int mypipe_destroy(mypipe_t *pipe);

#endif