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

int main (int argc, char* argv[]) {
    struct sockaddr_in servaddr = {0};
    bzero (&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (13);

    int listenfd = socket (AF_INET, SOCK_STREAM, 0);
    CHECK_ERR (listenfd, );

    #define ERR_ACT close (listenfd)

    CHECK_ERR (bind (listenfd, (SA*) &servaddr, sizeof (servaddr)), ERR_ACT);
    CHECK_ERR (listen (listenfd, 20), ERR_ACT);

    struct sockaddr addr = {0};
    socklen_t addrlen = 0;
    char buf[BUF_SIZE];
    while (true) {

        int connfd = accept (listenfd, &addr, &addrlen);
        CHECK_ERR (connfd, close (listenfd));
        printf ("Good accept!\n");
        
        time_t ticks = time (NULL);
        CHECK_ERR (snprintf (buf, sizeof (buf), "%.24s\r\n", ctime (&ticks)), ERR_ACT);

        printf ("Start write: %s\n", buf);
        CHECK_ERR (Write (connfd, buf, strlen (buf)), ERR_ACT);
        printf ("End write\n");

        CHECK_ERR (close (connfd), ERR_ACT);
    }

    CHECK_ERR (close (listenfd), );
    printf ("Connect closed\n");
}