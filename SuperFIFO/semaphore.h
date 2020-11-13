#ifndef SUPERFIFO_SEMAPHORE_H
#define SUPERFIFO_SEMAPHORE_H

typedef struct {
    int sem_fd[2];
    int sem_magic;
} mysem_t;

#define SEM_MAGIC 0x87654321
#define SEM_FAILED ((mysem_t *)(-1))

mysem_t *mysem_open (const char *pathname, int oflag, mode_t mode, unsigned int value);
int mysem_close (mysem_t *sem);
int mysem_unlink (const char *pathname);
int mysem_post (mysem_t *sem);
int mysem_wait (mysem_t *sem);

#endif //SUPERFIFO_SEMAPHORE_H
