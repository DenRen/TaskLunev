#ifndef FWRONSEMSHM_WRRD_H
#define FWRONSEMSHM_WRRD_H

const int MAGIC_NUMBER = 5;
const int NUMBER_PERS_SEM = 3;
const int SIZE_BUF = 4096;
const int DELAY_SECONDS = 5;

#undef EMPTY    // utmp.h have EMPTY!!!

struct CONN
{
    enum SEM
    {
        EMPTY, MUTEX, FULL, W_READED, SIZE
    };
};

struct PERS
{
    enum SEM
    {
        EMPTY, FULL, SIZE
    };
};

typedef struct
{
    int semID;
    int readerShmID;
}
connShm_t;

typedef struct
{
    int semID;
    int currSizeData;
    char buf[SIZE_BUF];
}
persShm_t;

union semun {
    int val;                  /* value for SETVAL */
    struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;    /* array for GETALL, SETALL */
    struct seminfo *__buf;    /* buffer for IPC_INFO */
};

int writer (connShm_t *connShm, char nameFile[]);
int reader (connShm_t *connShm);
int InitPersSem (int persSemID);
int InitConnShm (const char nameConnShm[]);
connShm_t *GetConnShm (const char nameFile[]);

sembuf GetOperation (unsigned short int num, short int op, short int flag);
int P (int semID, unsigned short int semNum, short int flag = 0);
int V (int semID, unsigned short int semNum, short int flag = 0);

int Ptime (int semID, unsigned short int semNum, short int flag = 0, int seconds = 0);
int Vtime (int semID, unsigned short int semNum, short int flag = 0, int seconds = 0);



#endif //FWRONSEMSHM_WRRD_H