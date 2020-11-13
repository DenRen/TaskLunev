#include <cstdio>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "AddFunc.h"

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf ("Input failed\n");
        return EXIT_FAILURE;
    }

    setvbuf (stdout, nullptr, _IONBF, 0);

    char *error = nullptr;
    errno = 0;
    int numberChilds = strtoll (argv[1], &error, 10);
    CHECK_TRUE (error == nullptr && errno == 0);

    key_t msqid = msgget (IPC_PRIVATE, IPC_CREAT | 0600);
    CHECK_ERROR (msqid);

    int number = -1;
    for (int i = 1; i <= numberChilds; i++)
    {
        pid_t pid = fork ();
        CHECK_ERROR (pid);
        if (pid == 0)   // Child
        {
            number = i;
            break;
        }
    }

    int result = 0;

    if (number != -1)   // Child
    {
        long msgBuf = 0;
        if (number != 1) // First child
        {
            result = msgrcv (msqid, &msgBuf, 0, number - 1, 0);
            CHECK_ERROR (result);
        }

        printf ("%d ", number); // Это крит. секция: дети разделяют вывод в stdout

        long msgType = number;
        result = msgsnd (msqid, (void *) &msgType, 0, 0);
        CHECK_ERROR (result);

        return EXIT_SUCCESS;
    }
    else
    {
        long msgType = 0;
        if (numberChilds > 0)
        {
            result = msgrcv (msqid, (void *) &msgType, 0, numberChilds, 0);
            CHECK_ERROR (result);
        }
        printf ("\n");
        result = msgctl (msqid, IPC_RMID, nullptr);
        CHECK_ERROR (result);
    }
}