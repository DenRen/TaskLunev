#include "learn/unp.h"
#include "lib/hpc_net.h"
#include <math.h>

double func (double x) {
    return x * x;
}

void ErrorInput (char* argv[]) {
    printf ("1) For start client, enter: %s\n"
            "2) For start server, enter: %s server %%numclients %%numthreads\n", argv[0], argv[0]);
}

int main (int argc, char* argv[]) {
    if (argc == 1)
    { 
        // Start TCP client and connect to server
        int clifd = StartClient ();
        CHECK_ERR (clifd, );

        CHECK_ERR (hpcnetStartComputeUnit (clifd), close (clifd));
        close (clifd);
    }
    else if (argc == 4 && strcmp (argv[1], KEY_START_SERVER) == 0)
    {
        const unsigned num_threads = atoi (argv[3]);
        serv_attr_t serv_attr = {
            .clients_limit = atoi (argv[2]),
            .if_name       = "wlo1",    // Wi-Fi
            .sa_family     = AF_INET    // IPv4
        };
        serv_param_t serv_param = {0};
        double result = 0.0;

        if (num_threads == 0 || serv_attr.clients_limit == 0)
            CHECK_ERR (-1, ErrorInput (argv));

        // Satrt TCP server with connected clients
        CHECK_ERR (StartServer (&serv_attr, &serv_param), );
        CHECK_ERR (hpcnetCalcIntegral (&serv_param, num_threads, func, 0, 90, 1e-10, &result),
                   CloseServer (&serv_param));
        CHECK_ERR (CloseServer (&serv_param), );

        printf ("Result: %g\n", result);
    }
    else
    {
        ErrorInput (argv);
    }

    return 0;
}