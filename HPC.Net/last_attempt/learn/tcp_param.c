#include "unp.h"
#include <netinet/tcp.h>

union val {
    int     i_val;
    long    l_val;
    struct linger linger_val;
    struct timeval timeval_val;
} val;

static char *sock_str_flag (union val* ptr, int len) {
    static char strres[128];

    if (len != sizeof (int))
        snprintf (strres, sizeof (strres), "size (%d) not sizeof (int)", len);
    else
        snprintf (strres, sizeof (strres), "%s", (ptr->i_val == 0) ? "off" : "on");

    return strres;
}

struct sock_opts {
    const char* opt_str;
    int         opt_level;
    int         opt_name;
    char        *(*opt_val_str) (union val*, int);
} sock_opt[] = {
    {"SO_BROADCAST", SOL_SOCKET, SO_BROADCAST,  sock_str_flag},
    {"SO_DEBUG",     SOL_SOCKET, SO_DEBUG,      sock_str_flag},
    {NULL,           0,          0,             NULL}
};

int main (int argc, char* argv[]) {
    int fd = 0;
    socklen_t len = 0;

    for (struct sock_opts* ptr = sock_opt; ptr->opt_str != NULL; ++ptr) {
        printf ("%s: ", ptr->opt_str);
        if (ptr->opt_val_str == NULL)
            printf ("(undefined)\n");
        else {
            switch (ptr->opt_level) {
            case SOL_SOCKET:
            case IPPROTO_IP:
            case IPPROTO_TCP:
                CHECK_ERR (fd = socket (AF_INET, SOCK_STREAM, 0), );
                break;
            default:
                CHECK_ERR (-1, );
            }
        }
        
        #define ERR_ACT close (fd);

        len = sizeof (val);
        CHECK_ERR (getsockopt (fd, ptr->opt_level, ptr->opt_name, &val, &len), ERR_ACT);

        printf ("default = %s\n", (*ptr->opt_val_str) (&val, len));
        
        #undef ERR_ACT
        close (fd);
    }
}