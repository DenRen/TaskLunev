#include <cerrno>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>

#include "semaphore.h"

mysem_t *mysem_open (const char *pathname, int oflag, mode_t mode, unsigned int value) {
    int i = 0, flags = 0, save_errno = 0;
    char c = '\0';
    mysem_t *sem = nullptr;
    if (oflag & O_CREAT) {
        if (mkfifo (pathname, mode) < 0) {
            if (errno == EEXIST && (oflag & O_EXCL) == 0)
                oflag &= ~O_CREAT;
            else
                return SEM_FAILED;
        }
    }

    if ((sem = (mysem_t *) calloc (1, sizeof (mysem_t))) == nullptr)
        return SEM_FAILED;

    sem->sem_fd[0] = sem->sem_fd[1] = -1;
    if ((sem->sem_fd[0] = open (pathname, O_RDONLY | O_NONBLOCK)) < 0)
        goto error;

    if ((sem->sem_fd[1] = open (pathname, O_WRONLY | O_NONBLOCK)) < 0)
        goto error;

    if ((flags = fcntl (sem->sem_fd[0], F_GETFL, 0)) < 0)
        goto error;
    flags &= ~O_NONBLOCK;
    if (fcntl (sem->sem_fd[0], F_SETFL, flags) < 0)
        goto error;

    if (oflag & O_CREAT) {
        for (i = 0; i < value; i++)
            if (write (sem->sem_fd[1], &c, 1) != 1)
                goto error;
    }

    sem->sem_magic = SEM_MAGIC;
    return sem;

    error:
    save_errno = errno;
    if (oflag & O_CREAT)
        unlink (pathname);
    close (sem->sem_fd[0]);
    close (sem->sem_fd[1]);
    free (sem);
    errno = save_errno;
    return SEM_FAILED;
}

int mysem_close (mysem_t *sem) {
    if (sem->sem_magic != SEM_MAGIC) {
        errno = EINVAL;
        return -1;
    }

    sem->sem_magic = 0;
    if (close (sem->sem_fd[0] == -1) || close (sem->sem_fd[1] != -1)) {
        free (sem);
        return -1;
    }

    free (sem);
    return 0;
}

int mysem_unlink (const char *pathname) {
    return unlink (pathname);
}

int mysem_post (mysem_t *sem) {
    if (sem->sem_magic != SEM_MAGIC) {
        errno = EINVAL;
        return -1;
    }

    char c = '\0';
    if (write (sem->sem_fd[1], &c, 1) == 1)
        return 0;

    return -1;
}

int mysem_wait (mysem_t *sem) {
    if (sem->sem_magic != SEM_MAGIC) {
        errno = EINVAL;
        return -1;
    }

    char c = '\0';
    if (read (sem->sem_fd[0], &c, 1) == 1)
        return 0;

    return -1;
}