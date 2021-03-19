#include "unp.h"

int main (int argc, char* argv[]) {
    
    if (argc != 2) {
        printf ("Please, enter: %s <IPaddress>\n", argv[0]);
        return -1;
    }
    
    struct sockaddr_in servaddr = {0};
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (13);
    CHECK_ERR (inet_pton (AF_INET, argv[1], &servaddr.sin_addr) - 1,
        char str_err[MAX_LINE] = "";
        snprintf (str_err, sizeof (str_err), "inet_pton: %s", argv[1]);
    );
    
    int sockfd = 0;
    CHECK_ERR (sockfd = socket (AF_INET, SOCK_STREAM, 0), );
    CHECK_ERR (connect (sockfd, (SA*) &servaddr, sizeof (servaddr)), close (sockfd));

    char recvline[MAX_LINE + 1];
    int n = 0;
    while ((n = read (sockfd, recvline, MAX_LINE))) {
        CHECK_ERR (n, close (sockfd));

        recvline[n] = '\0';
        printf ("%s", recvline);
    }

    close (sockfd);

    if (n < 0) {
        PRINT_ERROR ("read");
        return -1;
    }

    return 0;
}