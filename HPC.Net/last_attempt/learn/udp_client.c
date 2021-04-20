#include "unp.h"

int diag_client (FILE* instream, int sockfd, SA* pservaddr, socklen_t servlen) {

    CHECK_ERR (connect (sockfd, (SA*) pservaddr, servlen), );

    char buf[MAX_LINE];

    while (fgets (buf, sizeof (buf), instream) != NULL) {
        CHECK_ERR (write (sockfd, buf, sizeof (buf)), );

        int num = read (sockfd, buf, sizeof (buf));
        CHECK_ERR (num, );
        buf[num] = '\0';

        fputs (buf, stdout);
    }

    return 0;
}

int main (int argc, char* argv[]) {
    
    char* serv_ip_str = NULL;
    if (argc != 2)
        serv_ip_str = serv_def_ip_str;
    else
        serv_ip_str = argv[1];
    
    struct sockaddr_in servaddr;
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (SERV_PORT);
    CHECK_ERR (inet_pton (AF_INET, serv_ip_str, &servaddr.sin_addr.s_addr), );

    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (sockfd, );

    #define ERR_ACT close (sockfd)

    CHECK_ERR (diag_client (stdin, sockfd, (SA*) &servaddr, sizeof (servaddr)), ERR_ACT);

    #undef ERR_ACT
    
    close (sockfd);
} 