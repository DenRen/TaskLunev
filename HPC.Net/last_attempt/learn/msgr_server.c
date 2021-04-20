#include "unp.h"
#include <time.h>

int Write (int fd, char* buf, size_t size) {
    if (fd < 0 || buf == NULL) {
        PRINT_ERROR ("fd < 0 || buf == NULL");
        return -1;
    }

    int num_written = 0;
    do {
        CHECK_ERR ((num_written = write (fd, buf, size)), );
    } while ((size -= num_written));

    return 0;
}

const int max_listen = 20;

int main (int argc, char* argv[]) {
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);

    int listenfd = socket (AF_INET, SOCK_STREAM, 0);
    CHECK_ERR (listenfd, );

    #define ERR_ACT close (listenfd)

    CHECK_ERR (bind (listenfd, (SA*) &servaddr, sizeof (servaddr)), ERR_ACT);
    
    CHECK_ERR (listen (listenfd, max_listen), ERR_ACT);

    printf ("Server started\n");

    char str_ip_addr[INET_ADDRSTRLEN];
    printf ("Server address: %s:%d\n", 
             inet_ntop (AF_INET, &servaddr.sin_addr.s_addr, str_ip_addr, sizeof (str_ip_addr)), 
             ntohs (servaddr.sin_port));

    char buf[BUF_SIZE];
    while (true) {
        
        struct sockaddr_in client_addr = {0};
        socklen_t addrlen = sizeof (client_addr);

        int connfd = accept (listenfd, (SA*) &client_addr, &addrlen);
        CHECK_ERR (connfd, ERR_ACT);
        
        printf ("Client address: %s:%d\n",
                 inet_ntop (AF_INET, &client_addr.sin_addr.s_addr, str_ip_addr, sizeof (str_ip_addr)),
                 ntohs (client_addr.sin_port));

        addrlen = sizeof (client_addr);
        CHECK_ERR (getpeername (connfd, (SA*) &client_addr, &addrlen), ERR_ACT);
        
        printf ("Client peer address: %s:%d\n",
                 inet_ntop (AF_INET, &client_addr.sin_addr.s_addr, str_ip_addr, sizeof (str_ip_addr)),
                 ntohs (client_addr.sin_port));

        struct sockaddr_in server_addr = {0};
        addrlen = sizeof (server_addr);
        CHECK_ERR (getsockname (connfd, (SA*) &server_addr, &addrlen), ERR_ACT);
        printf ("Server address: %s:%d\n",
                 inet_ntop (AF_INET, &server_addr.sin_addr.s_addr, str_ip_addr, sizeof (str_ip_addr)),
                 ntohs (server_addr.sin_port));

        time_t ticks = time (NULL);
        CHECK_ERR (snprintf (buf, sizeof (buf), "%.24s\r\n", ctime (&ticks)), ERR_ACT);

        printf ("Start write: %s\n", buf);
        CHECK_ERR (Write (connfd, buf, strlen (buf)), ERR_ACT);
        printf ("End write\n");

        CHECK_ERR (close (connfd), ERR_ACT);
    }

    #undef ERR_ACT

    CHECK_ERR (close (listenfd), );
    printf ("Server finished");
}