#include "unp.h"

const int port = 13;
char serv_str_ip[INET_ADDRSTRLEN] = "127.0.0.1";

int main (int argc, char* argv[]) {
    
    char* str_ip = serv_str_ip;

    if (argc == 2)
        str_ip = argv[1];
    
    struct sockaddr_in servaddr = {0};
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (13);
    CHECK_ERR (inet_pton (AF_INET, str_ip, &servaddr.sin_addr) - 1,
        char str_err[MAX_LINE] = "";
        snprintf (str_err, sizeof (str_err), "inet_pton: %s", argv[1]);
    );

    printf ("client_addr: %s\n", sock_ntop ((SA*) &servaddr, sizeof (servaddr)));

    int sockfd = 0;
    CHECK_ERR (sockfd = socket (AF_INET, SOCK_STREAM, 0), );

    #define ERR_ACT close (sockfd)
    CHECK_ERR (connect (sockfd, (SA*) &servaddr, sizeof (servaddr)), ERR_ACT);

    char str_ip_addr[256];
    struct sockaddr_in client_addr = {0};
    socklen_t addrlen = sizeof (client_addr);
    CHECK_ERR (getsockname (sockfd, (SA*) &client_addr, &addrlen), ERR_ACT);
    printf ("Client address: %s:%d\n",
             inet_ntop (AF_INET, &client_addr.sin_addr.s_addr, str_ip_addr, sizeof (str_ip_addr)),
             ntohs (client_addr.sin_port));

    char recvline[MAX_LINE + 1];
    int n = 0;
    while ((n = read (sockfd, recvline, MAX_LINE))) {
        CHECK_ERR (n, ERR_ACT);

        recvline[n] = '\0';
        printf ("%s", recvline);
    }

    CHECK_ERR (n, ERR_ACT);

    #undef ERR_ACT
    close (sockfd);
    printf ("Finish\n");
}