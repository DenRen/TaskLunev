#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include "/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.h"
#include "wrrd.h"

// Сдал вопрос о перестановке кода

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf ("Please, enter the file name\n");
        return -1;
    }

    int parentPID = getpid ();

    sigset_t fillSet = {0};
    sigfillset (&fillSet);
    CHECK_ERROR (sigprocmask (SIG_SETMASK, &fillSet, nullptr));

    int childPID = fork ();

    CHECK_ERROR (parentPID);
    CHECK_ERROR (childPID);

    int result = 0;
    if (childPID == 0)
    {
        result = writer (parentPID, argv[1]);
    }
    else
    {
        result = reader (childPID);
    }

    return result;
}
/*
    struct sigaction act = {};
    act.sa_handler = SigTERMHandler;
    sigfillset (&act.sa_mask);
    act.sa_flags = SA_RESTART;
    CHECK_ERROR (sigaction (SIGTERM, &act, nullptr));
    printf ("%d\n", SIGRTMAX);
    act.sa_handler = SigTTINHandler;
    act.sa_mask.__val[0] = 0;
    act.sa_flags = 0;
    CHECK_ERROR (sigaction (SIGTTIN, &act, nullptr));

    //siginfo_t

    printf ("mypid: %d\n", getpid ());

    int _temp = 0;
    int res = scanf ("%d", &_temp);*/
