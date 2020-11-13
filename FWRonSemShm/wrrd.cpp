#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include "wrrd.h"
#include "../AddLib/AddFunc.h"

#define connP(sem) CHECK_ERROR (P (connSemID, CONN::sem, SEM_UNDO))
#define connV(sem) CHECK_ERROR (V (connSemID, CONN::sem, SEM_UNDO))

#define readerP(sem) CHECK_ERROR (Ptime (readerSemID, PERS::sem, 0, DELAY_SECONDS))
#define readerV(sem) CHECK_ERROR (Vtime (readerSemID, PERS::sem, 0, DELAY_SECONDS))

#define persP(sem) CHECK_ERROR (Ptime (persSemID, PERS::sem, 0, DELAY_SECONDS))
#define persV(sem) CHECK_ERROR (Vtime (persSemID, PERS::sem, 0, DELAY_SECONDS))

int writer (connShm_t *connShm, char nameFile[])
{
    // Receive reader memory ID and connect to it -----------------------
    int connSemID = connShm->semID;

    connP (FULL);
    connP (MUTEX);

    int readerShmID = connShm->readerShmID;
    persShm_t *readerShm = (persShm_t *) shmat (readerShmID, nullptr, 0666);
    CHECK_NULLPTR (readerShm);

    int readerSemID = readerShm->semID;

    connV (MUTEX);
    connV (W_READED);

    _ {
        unsigned short state[4] = {0};
        CHECK_ERROR (semctl (connShm->semID, 0, GETALL, state));
        for (int i = 0; i < 4; i++)
            printf ("%d ", state[i]);
        fflush (stdout);
    }
    // Write data ------------------------------------------------------

    int fd = open (nameFile, O_RDONLY);
    CHECK_ERROR (fd);

    do {
        readerP (EMPTY);

        CHECK_ERROR (readerShm->currSizeData = read (fd, readerShm->buf, SIZE_BUF));

        readerV (FULL);
    } while (readerShm->currSizeData > 0);

    // Disconnect from reader shared memory -----------------------------
    CHECK_ERROR (shmdt (readerShm));

    return EXIT_SUCCESS;
}

// TODO При любой ошибке остаётся выделенная разделяемая память, это нужно будет потом обрабатывать
int reader (connShm_t *connShm)
{
    // Initialize personal shared memory and semaphores -----------------
    int persShmID = shmget (IPC_PRIVATE, sizeof (persShm_t), 0666);
    CHECK_ERROR (persShmID);

    persShm_t *persShm = (persShm_t *) shmat (persShmID, nullptr, 0);
    CHECK_NULLPTR (persShm);

    int persSemID = semget (IPC_PRIVATE, PERS::SEM::SIZE, 0666);
    CHECK_ERROR (persSemID);
    persShm->semID = persSemID;

    CHECK_ERROR (InitPersSem (persSemID));

    // Send personal memory ID ------------------------------------------
    int connSemID = connShm->semID;

    connP (EMPTY);
    connP (MUTEX);
    connShm->readerShmID = persShmID;
    connV (MUTEX);
    connV (FULL);
    connP (W_READED);
    ushort ops[CONN::SIZE] = {0};
    ops[CONN::EMPTY]    = 1;
    ops[CONN::MUTEX]    = 1;
    ops[CONN::FULL]     = 0;
    ops[CONN::W_READED] = 0;
    CHECK_ERROR (semctl (connSemID, 0, SETALL, ops));

    _ {
        unsigned short state[4] = {0};
        CHECK_ERROR (semctl (connShm->semID, 0, GETALL, state));
        for (int i = 0; i < 4; i++)
            printf ("%d ", state[i]);
        fflush (stdout);
    }
    // Read and print data ----------------------------------------------

    do {
        persP (FULL);
        CHECK_ERROR (write (STDOUT_FILENO, persShm->buf, persShm->currSizeData));

        persV (EMPTY);

    } while (persShm->currSizeData > 0);

    // Delete personal shared memory and semaphores ---------------------
    CHECK_ERROR (semctl (persSemID, 0, IPC_RMID));
    CHECK_ERROR (shmdt (persShm));
    CHECK_ERROR (shmctl (persShmID, 0, IPC_RMID));

    return EXIT_SUCCESS;
}

int InitPersSem (int persSemID)
{
    unsigned short values[PERS::SIZE] = {0};
    values[PERS::EMPTY] = 1;
    values[PERS::FULL] = 0;

    CHECK_ERROR (semctl (persSemID, 0, SETALL, values));

    return EXIT_SUCCESS;
}

int InitConnShm (const char nameConnShm[])
{
    int connShmID = shmget (ftok (nameConnShm, MAGIC_NUMBER), sizeof (connShm_t), IPC_CREAT | 0666);
    CHECK_ERROR (connShmID);
    _ printf ("connShmID: %d\n", connShmID);

    connShm_t *connShm = (connShm_t *) shmat (connShmID, nullptr, 0);
    CHECK_NULLPTR (connShm);

    int semID = semget (ftok (nameConnShm, MAGIC_NUMBER), CONN::SEM::SIZE, IPC_CREAT | 0666);
    CHECK_ERROR (semID);
    _ printf ("connSemID: %d\n", semID);

    ushort ops[CONN::SIZE] = {0};
    ops[CONN::EMPTY]    = 1;
    ops[CONN::MUTEX]    = 1;
    ops[CONN::FULL]     = 0;
    ops[CONN::W_READED] = 0;
    CHECK_ERROR (semctl (semID, 0, SETALL, ops));

    connShm->semID = semID;

    CHECK_ERROR (shmdt (connShm));

    return EXIT_SUCCESS;
}

int P (int semID, unsigned short int semNum, short int flag)
{
    sembuf semBuf = {0};
    semBuf.sem_num = semNum;
    semBuf.sem_op = -1;
    semBuf.sem_flg = flag;

    CHECK_ERROR (semop (semID, &semBuf, 1));

    return EXIT_SUCCESS;
}

int V (int semID, unsigned short int semNum, short int flag)
{
    sembuf semBuf = {0};
    semBuf.sem_num = semNum;
    semBuf.sem_op = +1;
    semBuf.sem_flg = flag;

    CHECK_ERROR (semop (semID, &semBuf, 1));

    return EXIT_SUCCESS;
}

int Ptime (int semID, unsigned short int semNum, short int flag, int seconds)
{
    sembuf semBuf = {0};
    semBuf.sem_num = semNum;
    semBuf.sem_op = -1;
    semBuf.sem_flg = flag;

    timespec _timeout = {0};
    _timeout.tv_sec = seconds;

    CHECK_ERROR (semtimedop (semID, &semBuf, 1, &_timeout));

    return EXIT_SUCCESS;
}
int Vtime (int semID, unsigned short int semNum, short int flag, int seconds)
{
    sembuf semBuf = {0};
    semBuf.sem_num = semNum;
    semBuf.sem_op = +1;
    semBuf.sem_flg = flag;

    timespec _timeout = {0};
    _timeout.tv_sec = seconds;

    CHECK_ERROR (semtimedop (semID, &semBuf, 1, &_timeout));

    return EXIT_SUCCESS;
}

sembuf GetOperation (unsigned short int num, short int op, short int flag)
{
    sembuf semBuf = {0};
    semBuf.sem_num = num;
    semBuf.sem_op = op;
    semBuf.sem_flg = flag;

    return semBuf;
}

connShm_t *GetConnShm (const char nameFile[])
{
    int connShmID = shmget (ftok (nameFile, MAGIC_NUMBER), sizeof (connShm_t), 0666);
    if (connShmID == -1)
    {
        printf ("Please start the init first\n");
        PRINT_ERROR_MSG;
        return nullptr;
    }

    connShm_t *connShm = (connShm_t *) shmat (connShmID, nullptr, 0);
    if (connShm == nullptr)
    {
        PRINT_ERROR_MSG;
        return nullptr;
    }


    int connSemID = semget (ftok (nameFile, MAGIC_NUMBER), CONN::SIZE, 0666);
    if (connSemID == -1)
    {
        PRINT_ERROR_MSG;
        return nullptr;
    }

    if (connShm->semID == connSemID)
    {
        return connShm;
    }
    else
    {
        return nullptr;
    }
}