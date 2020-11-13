#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>
#include "../AddLib/AddFunc.h"
#include "wrrd.h"

int main (int argc, char *argv[])
{
    if (argc > 4)
    {
        printf ("Failed input\n");
        RETURN_EXIT_FAILED;
    }

    const char *nameFile = argv[0];

    if (argc == 3)
    {
        CHECK_ERROR (InitConnShm (nameFile));
        _ printf ("Initialize complete\n");
    }

    connShm_t *connShm = GetConnShm (nameFile);
    CHECK_NULLPTR (connShm);
    // After that, we know for sure that the connect
    // shared memory has been created and initialized.

    if (argc >= 3)
    {
        printf ("connShmID: %d\n", shmget (ftok (nameFile, MAGIC_NUMBER), sizeof (connShm_t), 0666));
        printf ("connSemID: %d\n", connShm->semID);
        unsigned short state[4] = {0};
        CHECK_ERROR (semctl (connShm->semID, 0, GETALL, state));
        for (int i = 0; i < 4; i++)
            printf ("%d ", state[i]);
        printf ("\n");
        fflush (stdout);

        return 0;
    }

    int result = 0;

    if (argc == 2)
        result = writer (connShm, argv[1]);
    else
        result = reader (connShm);

    CHECK_ERROR (shmdt (connShm));

    return result;
/*
    const int numSem = 3;
    int semID = semget (IPC_PRIVATE, numSem, 0666);
    CHECK_ERROR (semID);
    _ printf ("semID: %d\n", semID);

    sembuf arg = GetOperation (0, 256, 0);
    CHECK_ERROR (semop (semID, &arg, 1));

    _ printf ("mypid: %d\n", getpid ());

    int result = 0;
    printf ("%d\n", semctl (semID, 0, GETVAL));

    CHECK_ERROR (semctl (semID, 0, IPC_RMID));
*/
/*
    const int shmSize = 1024;
    //int shmid = shmget (IPC_PRIVATE, shmSize, 0666);
    int shmid = 30113810;
    CHECK_ERROR (shmid);
    _ printf ("shmid: %d\n", shmid);

    semid_ds

    int *mem = (int *) shmat (shmid, nullptr, 0);
    CHECK_NULLPTR (mem);
    _ printf ("mem: %p\n", mem);

    CHECK_ERROR (shmctl (shmid, IPC_RMID, nullptr));

    CHECK_ERROR (shmdt (mem));*/
}
