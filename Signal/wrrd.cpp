#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include "wrrd.h"
#include "../AddLib/AddFunc.h"

bool curBit = false;
bool sigChild = false;

void sigHandlerReaderUSR1 (int)
{
    curBit = false;
}

void sigHandlerReaderUSR2 (int)
{
    curBit = true;
}

void sigHandlerReaderCHLD (int)
{
    sigChild = true;
}

int reader (int writerPID)
{
    // Initialize handler SIGCHLD, SIGUSR1 and SIGUSR2 ------------------

    struct sigaction saUSR1 = {0};
    saUSR1.sa_handler = sigHandlerReaderUSR1;
    sigfillset (&saUSR1.sa_mask);
    saUSR1.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGUSR1, &saUSR1, nullptr));

    struct sigaction saUSR2 = {0};
    saUSR2.sa_handler = sigHandlerReaderUSR2;
    sigfillset (&saUSR2.sa_mask);
    saUSR2.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGUSR2, &saUSR2, nullptr));

    struct sigaction saCHILD = {0};
    saCHILD.sa_handler = sigHandlerReaderCHLD;
    sigfillset (&saCHILD.sa_mask);
    saCHILD.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGCHLD, &saCHILD, nullptr));

    // ------------------------------------------------------------------

    sigset_t maskConfirm = {0};
    sigemptyset (&maskConfirm);

    char buf = '\0';
    while (true)
    {
        buf = 0;
        for (int i = 0; i < 8; i++)
        {
            sigsuspend (&maskConfirm);
            if (sigChild)
            {
                return 0;
            }

            buf |= curBit << i;

            CHECK_ERROR (kill (writerPID, SIGUSR1));
        }

        write (STDOUT_FILENO, &buf, 1); // Строчка к.с., т.к. разные процессы могут разделять один и тот же поток вывода
    }
}

bool timeConfReaderOver = false;

void sigHandlerWriterALARM (int)
{
    timeConfReaderOver = true;
}

void sigHandlerWriterUSR1 (int)
{

}

int writer (int readerPID, char *fileName)
{
    // Initialise handler SIGUSR1 ---------------------------------------

    struct sigaction saUSR1 = {0};
    saUSR1.sa_handler = sigHandlerWriterUSR1;
    sigfillset (&saUSR1.sa_mask);
    saUSR1.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGUSR1, &saUSR1, 0));

    struct sigaction saALARM = {0};
    saALARM.sa_handler = sigHandlerWriterALARM;
    sigfillset (&saALARM.sa_mask);
    saALARM.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGALRM, &saALARM, 0));

    // ------------------------------------------------------------------

    sigset_t maskConfirm = {0};
    sigfillset (&maskConfirm);
    sigdelset (&maskConfirm, SIGUSR1);
    sigdelset (&maskConfirm, SIGALRM);

    int fd = open (fileName, O_RDONLY);
    CHECK_ERROR (fd);

    char buf[SIZE_BUF] = "";
    int curSize = 0;
    // Начло к.с. между функцией read этого процесса и любой другой функцией другого, которая работает
    // с этим файлом на запись.
    // Например, какой-то процесс может писать в этот файл, либо этот файл вообще FIFO, открытый многими процессами
    // на запись, либо ещё что-то.
    while ((curSize = read (fd, &buf, SIZE_BUF)) != 0)
    {
        CHECK_ERROR (curSize);
        for (int j = 0; j < curSize; j++)
        {
            for (int i = 0; i < 8; i++)
            {
                alarm (2);

                if (buf[j] & 0b1)
                {
                    CHECK_ERROR (kill (readerPID, SIGUSR2));        // 1
                }
                else
                {
                    CHECK_ERROR (kill (readerPID, SIGUSR1));        // 0
                }

                buf[j] >>= 1;

                sigsuspend (&maskConfirm);
                if (timeConfReaderOver)
                {
                    printf ("Waiting time of reader out\n");
                    close (fd);

                    return -1;
                }
            }
        }
    }
    // Конец к.с.

    return curSize;
}

void bprint (unsigned char val)
{
    for (unsigned char i = 0b10000000; i > 0; i >>= 1)
        printf ("%d", (val & i) != 0);
}