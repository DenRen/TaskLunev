#include "learn/unp.h"
#include "lib/hpc_net.h"

/* Tasks:
    - Create interface selection field
*/

int main (int argc, char* argv[]) {
    if (argc == 1) {
        CHECK_ERR (StartClient (), );
    }
    else if (argc == 2 && strcmp (argv[1], KEY_START_SERVER) == 0) {
        const int clients_limit = 20;
        const char* if_name = "wlo1";       // Wi-Fi
        const sa_family_t family = AF_INET; // IPv4

        CHECK_ERR (StartServer (clients_limit, if_name, family), );   
    }
    else {
        printf ("1) For start client, enter: %s\n"
                "2) For start server, enter: %s server\n", argv[0], argv[0]);
    }

    return 0;
}