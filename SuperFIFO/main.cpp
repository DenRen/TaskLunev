#include <cstdio>
#include <cstring>
#include "SmartFIFO.h"
#include "wrrd.h"

// К.с. - фрагмент кода, параллельное исполнение которого, приводит к недетерминированному результату
// https://youtu.be/5I2c8Tes-iQ?list=PLthfp5exSWEqvuRGe_kmqA1qXiPMQRrJO&t=1385

int main (int argc, char *argv[]) {
    const char conn_str[MAXLEN] = "/tmp/connFIFO";

    CHECK_ERROR (CreateFIFO (conn_str));

    int ret = 0;
    if (argc == 2)
        ret = writer (conn_str, argv[1]);
    else if (argc == 1)
        ret = reader (conn_str);
    else
    {
        printf ("Key invalid");
        return EXIT_FAILURE;
    }

    if (ret)
        perror ("EXIT_FAILURE");

    return ret;
}
