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
    long msgType = 0;

    if (number != -1)   // Child
    {
        if (number != 1) // First child
        {
            result = msgrcv (msqid, &msgType, 0, number - 1, 0);
            CHECK_ERROR (result);
        }

        printf ("%d ", number);

        msgType = number;
        result = msgsnd (msqid, (void *) &msgType, 0, 0);
        CHECK_ERROR (result);

        return EXIT_SUCCESS;
    }
    else
    {
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