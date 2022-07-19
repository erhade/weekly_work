#ifndef SEMAPHORE_H__
#define SEMAPHORE_H__

typedef void sem_t;

sem_t *semaphore_init(int init_val);

int semaphore_add(sem_t *sem, int add_val);

int semaphore_sub(sem_t *sem, int sub_val);

int semaphore_destroy(sem_t *sem);

#endif